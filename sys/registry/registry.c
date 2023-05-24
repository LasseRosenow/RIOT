/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry RIOT Registry
 * @ingroup     module_registry
 * @brief       RIOT Registry for runtime configuration of modules
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "clist.h"

#include "registry.h"
#include "registry/util.h"

/* Implementation of the module */

clist_node_t _registry_namespaces;

int registry_register_namespace(const registry_namespace_t *namespace)
{
    assert(namespace != NULL);

    /* add namespace to list */
    clist_rpush((clist_node_t *)&_registry_namespaces, (clist_node_t *)&namespace->node);

    return 0;
}

int registry_register_schema_instance(const registry_schema_t *schema,
                                      const registry_instance_t *instance)
{
    assert(schema != NULL);
    assert(instance != NULL);

    /* add instance to schema */
    clist_rpush((clist_node_t *)&schema->instances, (clist_node_t *)&instance->node);

    return 0;
}

int registry_get(const registry_schema_t *schema, const registry_instance_t *instance,
                 const registry_resource_t *parameter, registry_value_t *value)
{
    assert(schema != NULL);
    assert(instance != NULL);
    assert(parameter != NULL);
    assert(value != NULL);

    /* call handler to get pointer to registry internal value buffer and length */
    void *intern_val = NULL;
    size_t intern_val_len;

    schema->mapping(parameter->id, instance, &intern_val, &intern_val_len);

    /* update buf pointer in registry_value_t to point to the value inside the registry and set buf_len */
    value->type = parameter->type;
    value->buf = intern_val;
    value->buf_len = intern_val_len;

    return 0;
}

int registry_set(const registry_schema_t *schema, const registry_instance_t *instance,
                 const registry_resource_t *parameter, const registry_value_t *value)
{
    assert(schema != NULL);
    assert(instance != NULL);
    assert(parameter != NULL);
    assert(value != NULL);

    /* get pointer to registry internal value buffer and length */
    void *intern_val = NULL;
    size_t intern_val_len;

    schema->mapping(parameter->id, instance, &intern_val, &intern_val_len);

    /* check if val_type is compatible with param_meta->value.parameter.type */
    if (value->type != parameter->type) {
        return -EINVAL;
    }
    else {
        /* call handler to apply the new value to the correct parameter in the instance of the schema */
        memcpy(intern_val, value->buf, intern_val_len);
    }

    return 0;
}

int registry_commit(void)
{
    int rc = 0;

    /* commit all namespaces */
    clist_node_t *node = _registry_namespaces.next;

    if (!node) {
        return -EINVAL;
    }

    do {
        node = node->next;
        registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

        if (!namespace) {
            return -EINVAL;
        }

        int _rc = registry_commit_namespace(namespace);

        if (!_rc) {
            rc = _rc;
        }
    } while (node != _registry_namespaces.next);

    return rc;
}

int registry_commit_namespace(const registry_namespace_t *namespace)
{
    assert(namespace != NULL);

    int rc = 0;

    /* commit all configuration schemas of the given namespace if available */
    for (size_t i = 0; i < namespace->schemas_len; i++) {
        const registry_schema_t *child = namespace->schemas[i];

        int _rc = registry_commit_schema(child);

        if (!_rc) {
            rc = _rc;
        }
    }

    return rc;
}

int registry_commit_schema(const registry_schema_t *schema)
{
    assert(schema != NULL);

    int rc = 0;

    /* commit all configuration schema instances of the given configuration schema if available */
    clist_node_t *node = schema->instances.next;

    if (!node) {
        return -EINVAL;
    }

    do {
        node = node->next;
        registry_instance_t *instance = container_of(node, registry_instance_t, node);

        if (!instance) {
            return -EINVAL;
        }

        int _rc = registry_commit_instance(instance);

        if (!_rc) {
            rc = _rc;
        }
    } while (node != schema->instances.next);

    return rc;
}

int registry_commit_instance(const registry_instance_t *instance)
{
    return instance->commit_cb(REGISTRY_COMMIT_INSTANCE, NULL, instance->context);
}

int registry_commit_group(const registry_instance_t *instance, const registry_resource_t *group)
{
    return instance->commit_cb(REGISTRY_COMMIT_GROUP, &group->id, instance->context);
}

int registry_commit_parameter(const registry_instance_t *instance,
                              const registry_resource_t *parameter)
{
    return instance->commit_cb(REGISTRY_COMMIT_PARAMETER, &parameter->id, instance->context);
}

int registry_export(const registry_export_cb_t export_cb, const int recursion_depth,
                    const void *context)
{
    int rc = 0;

    /* export all namespaces */
    clist_node_t *node = _registry_namespaces.next;

    if (!node) {
        return -EINVAL;
    }

    do {
        node = node->next;
        registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

        if (!namespace) {
            return -EINVAL;
        }

        int _rc = registry_export_namespace(namespace, export_cb, recursion_depth, context);

        if (!_rc) {
            rc = _rc;
        }
    } while (node != _registry_namespaces.next);

    return rc;
}

int registry_export_namespace(const registry_namespace_t *namespace,
                              const registry_export_cb_t export_cb, const int recursion_depth,
                              const void *context)
{
    assert(namespace != NULL);

    /* export the given namespace */
    registry_export_cb_data_t export_data = { .namespace = namespace };
    int rc = export_cb(&export_data, REGISTRY_EXPORT_NAMESPACE, context);

    /* export all configuration schemas of the given namespace if available and within recursion_depth bounds */
    if (recursion_depth == 1) {
        return 0;
    }
    else {
        int new_recursion_depth = recursion_depth;
        if (recursion_depth > 1) {
            new_recursion_depth--;
        }

        for (size_t i = 0; i < namespace->schemas_len; i++) {
            const registry_schema_t *child = namespace->schemas[i];

            int _rc = registry_export_schema(child, export_cb, new_recursion_depth, context);

            if (!_rc) {
                rc = _rc;
            }
        }
    }

    return rc;
}

int registry_export_schema(const registry_schema_t *schema, const registry_export_cb_t export_cb,
                           const int recursion_depth, const void *context)
{
    assert(schema != NULL);

    /* export the given configuration schema */
    registry_export_cb_data_t export_data = { .schema = schema };
    int rc = export_cb(&export_data, REGISTRY_EXPORT_SCHEMA, context);

    /* export all instances of the given configuration schema if available and within recursion_depth bounds */
    if (recursion_depth == 1) {
        return 0;
    }
    else {
        int new_recursion_depth = recursion_depth;
        if (recursion_depth > 1) {
            new_recursion_depth--;
        }

        clist_node_t *node = schema->instances.next;

        if (!node) {
            return -EINVAL;
        }

        do {
            node = node->next;
            registry_instance_t *instance = container_of(node, registry_instance_t, node);

            if (!instance) {
                return -EINVAL;
            }

            int _rc = registry_export_instance(schema, instance, export_cb, new_recursion_depth,
                                               context);

            if (!_rc) {
                rc = _rc;
            }
        } while (node != schema->instances.next);
    }

    return rc;
}

int registry_export_instance(const registry_schema_t *schema, const registry_instance_t *instance,
                             const registry_export_cb_t export_cb, const int recursion_depth,
                             const void *context)
{
    assert(schema != NULL);
    assert(instance != NULL);

    /* export the given configuration schema instance */
    registry_export_cb_data_t export_data = { .instance = instance };
    int rc = export_cb(&export_data, REGISTRY_EXPORT_INSTANCE, context);

    /* export all groups or parameters of the given configuration schema instance if available and within recursion_depth bounds */
    if (recursion_depth == 1) {
        return 0;
    }
    else {
        int new_recursion_depth = recursion_depth;
        if (recursion_depth > 1) {
            new_recursion_depth--;
        }

        int _rc = rc;

        for (size_t i = 0; i < schema->resources_len; i++) {
            const registry_resource_t *child = schema->resources[i];

            if (child->type == REGISTRY_TYPE_GROUP) {
                _rc = registry_export_group(child, export_cb, new_recursion_depth, context);
            }
            else {
                _rc = registry_export_parameter(child, export_cb, context);
            }

            if (!_rc) {
                rc = _rc;
            }
        }
    }

    return rc;
}

int registry_export_group(const registry_resource_t *group, const registry_export_cb_t export_cb,
                          const int recursion_depth, const void *context)
{
    assert(group != NULL);

    /* export the given configuration group */
    registry_export_cb_data_t export_data = { .group = group };
    int rc = export_cb(&export_data, REGISTRY_EXPORT_GROUP, context);

    /* export all children of the given configuration group if available and within recursion_depth bounds */
    if (recursion_depth == 1) {
        return 0;
    }
    else {
        int new_recursion_depth = recursion_depth;
        if (recursion_depth > 1) {
            new_recursion_depth--;
        }

        int _rc = rc;

        for (size_t i = 0; i < group->resources_len; i++) {
            const registry_resource_t *child = group->resources[i];

            if (child->type == REGISTRY_TYPE_GROUP) {
                _rc = registry_export_group(child, export_cb, new_recursion_depth, context);
            }
            else {
                _rc = registry_export_parameter(child, export_cb, context);
            }

            if (!_rc) {
                rc = _rc;
            }
        }
    }

    return rc;
}

int registry_export_parameter(const registry_resource_t *parameter,
                              const registry_export_cb_t export_cb, const void *context)
{
    assert(parameter != NULL);

    registry_export_cb_data_t export_data = { .parameter = parameter };
    return export_cb(&export_data, REGISTRY_EXPORT_PARAMETER, context);
}
