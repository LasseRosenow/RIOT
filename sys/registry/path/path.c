/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_path RIOT Registry Path
 * @ingroup     sys
 * @brief       RIOT Registry Path module providing a API to access configuration parameter via a integer path
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 * @author      Leandro Lanzieri <leandro.lanzieri@haw-hamburg.de>
 *
 * @}
 */

#include <errno.h>
#include <assert.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "registry.h"
#include "registry/util.h"
#include "registry/error.h"

#include "registry/path.h"

static const registry_resource_t *_resource_lookup(const registry_path_t *path,
                                                   const registry_schema_t *schema)
{
    const registry_resource_t *schema_resource;
    const registry_resource_t **schema_resources = schema->resources;
    size_t schema_resources_len = schema->resources_len;

    for (size_t i = 0; i < schema_resources_len; i++) {
        schema_resource = schema_resources[i];

        if (schema_resource->id == *path->resource_id) {
            return schema_resource;
        }
    }

    return NULL;
}

int registry_set_by_path(const registry_path_t *path, const registry_value_t *value)
{
    /* lookup namespace */
    const registry_namespace_t *namespace = registry_util_namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return -EINVAL;
    }

    /* lookup schema */
    const registry_schema_t *schema = registry_util_schema_lookup(namespace, *path->schema_id);

    if (!schema) {
        return -EINVAL;
    }

    /* lookup instance */
    const registry_instance_t *instance = registry_util_instance_lookup(schema, *path->instance_id);

    if (!instance) {
        return -EINVAL;
    }

    /* lookup parameter meta data */
    const registry_resource_t *parameter = _resource_lookup(path, schema);

    if (!parameter) {
        return -EINVAL;
    }
    else if (parameter->type == REGISTRY_TYPE_GROUP) {
        return -REGISTRY_ERROR_RESOURCE_TYPE_GROUP;
    }

    return registry_set(schema, instance, parameter, value);
}

int registry_get_by_path(const registry_path_t *path, registry_value_t *value)
{
    /* lookup namespace */
    const registry_namespace_t *namespace = registry_util_namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return -EINVAL;
    }

    /* lookup schema */
    const registry_schema_t *schema = registry_util_schema_lookup(namespace, *path->schema_id);

    if (!schema) {
        return -EINVAL;
    }

    /* lookup instance */
    const registry_instance_t *instance = registry_util_instance_lookup(schema, *path->instance_id);

    if (!instance) {
        return -EINVAL;
    }

    /* lookup parameter meta data */
    const registry_resource_t *parameter = _resource_lookup(path, schema);

    if (!parameter) {
        return -EINVAL;
    }
    else if (parameter->type == REGISTRY_TYPE_GROUP) {
        return -REGISTRY_ERROR_RESOURCE_TYPE_GROUP;
    }

    return registry_get(schema, instance, parameter, value);
}

static int _registry_commit_schema_by_path(const registry_path_t *path)
{
    int rc = 0;

    /* lookup namespace */
    const registry_namespace_t *namespace = registry_util_namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return -EINVAL;
    }

    /* lookup schema */
    const registry_schema_t *schema = registry_util_schema_lookup(namespace, *path->schema_id);

    if (!schema) {
        return -EINVAL;
    }

    /* schema/instance */
    if (path->instance_id != NULL) {
        /* lookup instance */
        const registry_instance_t *instance = registry_util_instance_lookup(schema,
                                                                            *path->instance_id);
        if (!instance) {
            return -EINVAL;
        }
        if (instance->commit_cb) {
            int _rc = instance->commit_cb(REGISTRY_COMMIT_INSTANCE, NULL, instance->context);
            if (!_rc) {
                rc = _rc;
            }
        }
        else {
            rc = -EINVAL;
        }
    }
    /* only schema */
    else {
        for (size_t i = 0; i < clist_count((clist_node_t *)&schema->instances); i++) {
            const registry_instance_t *instance = registry_util_instance_lookup(schema, i);
            if (instance->commit_cb) {
                int _rc =
                    instance->commit_cb(REGISTRY_COMMIT_INSTANCE, NULL, instance->context);
                if (!_rc) {
                    rc = _rc;
                }
            }
            else {
                rc = -EINVAL;
            }
        }
    }

    return rc;
}

static int _registry_commit_namespace_by_path(const registry_path_t *path)
{
    int rc = 0;

    /* lookup namespace */
    const registry_namespace_t *namespace = registry_util_namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return -EINVAL;
    }

    /* schema/? */
    if (path->schema_id != NULL) {
        int _rc = _registry_commit_schema_by_path(path);
        if (!_rc) {
            rc = _rc;
        }
    }
    /* no schema => call all */
    else {
        for (size_t i = 0; i < namespace->schemas_len; i++) {
            const registry_schema_t *schema = namespace->schemas[i];
            registry_path_t new_path = {
                .namespace_id = path->namespace_id,
                .schema_id = &schema->id,
                .instance_id = NULL,
                .resource_id = NULL,
            };

            int _rc =
                _registry_commit_schema_by_path(&new_path);
            if (!_rc) {
                rc = _rc;
            }
        }
    }

    return rc;
}

int registry_commit_by_path(const registry_path_t *path)
{
    int rc = 0;

    if (path->namespace_id != NULL) {
        int _rc = _registry_commit_namespace_by_path(path);
        if (!_rc) {
            rc = _rc;
        }
    }
    else {
        clist_node_t *node = _registry_namespaces.next;

        if (!node) {
            return -EINVAL;
        }

        registry_id_t index = 0;

        do {
            node = node->next;
            registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

            if (!namespace) {
                return -EINVAL;
            }

            /* create new path that includes the new namespace_id */
            registry_path_t new_path = {
                .namespace_id = &index,
                .schema_id = NULL,
                .instance_id = NULL,
                .resource_id = NULL,
            };

            int _rc = _registry_commit_namespace_by_path(&new_path);

            if (!_rc) {
                rc = _rc;
            }

            index++;
        } while (node != _registry_namespaces.next);
    }

    return rc;
}

static void _registry_export_params_by_path(const registry_path_export_cb_t export_cb,
                                            const registry_path_t *path,
                                            const registry_schema_t *schema,
                                            const void *context)
{
    const registry_resource_t *schema_resource = _resource_lookup(path, schema);

    /* check if the current schema_resource is a group or a parameter */
    if (schema_resource->type == REGISTRY_TYPE_GROUP) {
        /* group => search for parameters */
        registry_export_cb_data_t export_data = { .group = schema_resource };
        export_cb(path, &export_data, REGISTRY_EXPORT_GROUP, NULL, context);
    }
    else {
        /* parameter found => export */
        registry_value_t value;
        registry_get_by_path(path, &value);
        registry_export_cb_data_t export_data = { .parameter = schema_resource };
        export_cb(path, &export_data, REGISTRY_EXPORT_PARAMETER, &value, context);
    }
}

static int _registry_export_instance_by_path(const registry_path_export_cb_t export_cb,
                                             const registry_path_t *path,
                                             const registry_schema_t *schema,
                                             const int recursion_depth, const void *context)
{
    const registry_instance_t *instance = registry_util_instance_lookup(schema, *path->instance_id);

    if (!instance) {
        return -EINVAL;
    }

    /* export instance */
    registry_export_cb_data_t export_data = { .instance = instance };
    export_cb(path, &export_data, REGISTRY_EXPORT_INSTANCE, NULL, context);

    /* schema/instance/item => export concrete schema item with data of the given instance */
    if (path->resource_id != NULL) {
        _registry_export_params_by_path(export_cb, path, schema, context);
    }
    /* schema/instance => export the schema instance meta data (name) and its parameters recursively depending on recursion_depth */
    else {
        /* export instance parameters (recursion_depth == 1 means only the exact path, which would only be a specific instance in this case) */
        if (recursion_depth != 1) {
            int new_recursion_depth = 0;
            if (recursion_depth != 0) {
                new_recursion_depth = recursion_depth - 1;
            }

            _registry_export_params_by_path(export_cb, path, schema, context);
        }
    }

    return 0;
}

static int _registry_export_schema_by_path(const registry_path_export_cb_t export_cb,
                                           const registry_path_t *path, const int recursion_depth,
                                           const void *context)
{
    int rc = 0;

    /* lookup namespace */
    const registry_namespace_t *namespace = registry_util_namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return -EINVAL;
    }

    /* lookup schema */
    const registry_schema_t *schema = registry_util_schema_lookup(namespace, *path->schema_id);

    if (!schema) {
        return -EINVAL;
    }

    /* export schema */
    registry_export_cb_data_t export_data = { .schema = schema };
    export_cb(path, &export_data, REGISTRY_EXPORT_SCHEMA, NULL, context);

    /* get instance, if in path */
    if (path->instance_id != NULL) {
        int _rc =
            _registry_export_instance_by_path(export_cb, path, schema, recursion_depth, context);
        if (!_rc) {
            rc = _rc;
        }
    }
    /* schema => Export schema meta data (name, description etc.) and its items depending on recursion_depth */
    else {
        /* export instances (recursion_depth == 1 means only the exact path, which would only be a specific schema in this case) */
        if (recursion_depth != 1) {
            int new_recursion_depth = 0;
            if (recursion_depth != 0) {
                new_recursion_depth = recursion_depth - 1;
            }

            clist_node_t *node = schema->instances.next;

            if (!node) {
                return -EINVAL;
            }

            registry_id_t instance_id = 0;

            do {
                node = node->next;
                registry_instance_t *instance = container_of(node, registry_instance_t, node);

                if (!instance) {
                    return -EINVAL;
                }

                /* create new path that includes the new instance_id */
                registry_path_t new_path = {
                    .namespace_id = path->namespace_id,
                    .schema_id = path->schema_id,
                    .instance_id = &instance_id,
                    .resource_id = NULL,
                };

                int _rc = _registry_export_instance_by_path(export_cb, &new_path, schema,
                                                            new_recursion_depth,
                                                            context);
                if (!_rc) {
                    rc = _rc;
                }

                instance_id++;
            } while (node != schema->instances.next);
        }
    }

    return rc;
}

static int _registry_export_namespace_by_path(const registry_path_export_cb_t export_cb,
                                              const registry_path_t *path,
                                              const int recursion_depth, const void *context)
{
    int rc = 0;

    /* lookup namespace */
    const registry_namespace_t *namespace = registry_util_namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return -EINVAL;
    }

    /* export namespace */
    registry_export_cb_data_t export_data = { .namespace = namespace };
    export_cb(path, &export_data, REGISTRY_EXPORT_NAMESPACE, NULL, context);

    /* get schema, if in path */
    if (path->schema_id != NULL) {
        int _rc = _registry_export_schema_by_path(export_cb, path, recursion_depth, context);
        if (!_rc) {
            rc = _rc;
        }
    }
    /**
     * empty path => export everything depending on recursion_depth
     * (0 = everything, 1 = nothing, 2 = all schemas, 3 = all schemas and all their instances etc.)
     */
    else {
        if (recursion_depth != 1) {
            int new_recursion_depth = 0;
            if (recursion_depth != 0) {
                new_recursion_depth = recursion_depth - 1;
            }

            for (size_t i = 0; i < namespace->schemas_len; i++) {
                const registry_schema_t *schema = namespace->schemas[i];

                /* create new path that includes the new schema_id */
                registry_path_t new_path = {
                    .namespace_id = path->namespace_id,
                    .schema_id = &schema->id,
                    .instance_id = NULL,
                    .resource_id = NULL,
                };

                int _rc = _registry_export_schema_by_path(export_cb, &new_path,
                                                          new_recursion_depth,
                                                          context);
                if (!_rc) {
                    rc = _rc;
                }
            }
        }
    }

    return rc;
}

int registry_export_by_path(const registry_path_export_cb_t export_cb, const registry_path_t *path,
                            const int recursion_depth, const void *context)
{
    assert(export_cb != NULL);

    int rc = 0;

    DEBUG("[registry export] exporting all in ");
    // TODO DEBUG PRINT PATH FUNCTION
    DEBUG("\n");

    /* get namespace if in path */
    if (path->namespace_id != NULL) {
        int _rc = _registry_export_namespace_by_path(export_cb, path, recursion_depth, context);
        if (!_rc) {
            rc = _rc;
        }
    }
    else {
        if (recursion_depth != 1) {
            int new_recursion_depth = 0;
            if (recursion_depth != 0) {
                new_recursion_depth = recursion_depth - 1;
            }

            clist_node_t *node = _registry_namespaces.next;

            if (!node) {
                return -EINVAL;
            }

            registry_id_t index = 0;

            do {
                node = node->next;
                registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

                if (!namespace) {
                    return -EINVAL;
                }

                /* create new path that includes the new namespace_id */
                registry_path_t new_path = {
                    .namespace_id = &index,
                    .schema_id = NULL,
                    .instance_id = NULL,
                    .resource_id = NULL,
                };

                int _rc = _registry_export_namespace_by_path(export_cb, &new_path,
                                                             new_recursion_depth,
                                                             context);

                if (!_rc) {
                    rc = _rc;
                }

                index++;
            } while (node != _registry_namespaces.next);
        }
    }

    return rc;
}
