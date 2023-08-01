/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_int_path RIOT Registry Int Path
 * @ingroup     sys
 * @brief       RIOT Registry Int Path module providing a API to access configuration parameter via an integer path
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <errno.h>
#include <assert.h>
#include <stdlib.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "clist.h"
#include "registry.h"
#include "registry/util.h"
#include "registry/error.h"

#include "registry/int_path.h"

static const registry_namespace_t *_namespace_lookup(const registry_namespace_id_t namespace_id)
{
    clist_node_t *node = _registry_namespaces.next;

    if (!node) {
        return NULL;
    }

    do {
        node = node->next;
        const registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

        if (namespace->id == namespace_id) {
            return namespace;
        }
    } while (node != _registry_namespaces.next);

    return NULL;
}

static const registry_schema_t *_schema_lookup(const registry_namespace_t *namespace,
                                               const registry_schema_id_t schema_id)
{
    for (size_t i = 0; i < namespace->schemas_len; i++) {
        const registry_schema_t *schema = namespace->schemas[i];

        if (schema->id == schema_id) {
            return schema;
        }
    }

    return NULL;
}

static const registry_instance_t *_instance_lookup(const registry_schema_t *schema,
                                                   const registry_instance_id_t instance_id)
{
    assert(schema != NULL);

    /* find instance with correct instance_id */
    clist_node_t *node = schema->instances.next;

    if (!node) {
        return NULL;
    }

    do {
        node = node->next;
        const registry_instance_t *instance = container_of(node, registry_instance_t, node);

        /* check if index equals instance_id */
        if (instance->id == instance_id) {
            return instance;
        }
    } while (node != schema->instances.next);

    return NULL;
}

static const registry_group_t *_internal_group_lookup(const registry_group_t *group,
                                                      const registry_group_id_t group_id)
{
    for (size_t i = 0; i < group->groups_len; i++) {
        if (group->groups[i]->id == group_id) {
            return group->groups[i];
        }
        else if (group->groups[i]->groups_len > 0) {
            _internal_group_lookup(group->groups[i], group_id);
        }
    }

    return NULL;
}

static const registry_group_t *_group_lookup(const registry_schema_t *schema,
                                             const registry_group_id_t group_id)
{
    for (size_t i = 0; i < schema->groups_len; i++) {
        if (schema->groups[i]->id == group_id) {
            return schema->groups[i];
        }
        else if (schema->groups[i]->groups_len > 0) {
            _internal_group_lookup(schema->groups[i], group_id);
        }
    }

    return NULL;
}

static const registry_parameter_t *_internal_parameter_lookup(const registry_group_t *group,
                                                              const registry_parameter_id_t parameter_id)
{
    for (size_t i = 0; i < group->parameters_len; i++) {
        if (group->parameters[i]->id == parameter_id) {
            return group->parameters[i];
        }
    }

    for (size_t i = 0; i < group->groups_len; i++) {
        return _internal_parameter_lookup(group->groups[i], parameter_id);
    }

    return NULL;
}

static const registry_parameter_t *_parameter_lookup(const registry_schema_t *schema,
                                                     const registry_parameter_id_t parameter_id)
{
    for (size_t i = 0; i < schema->parameters_len; i++) {
        if (schema->parameters[i]->id == parameter_id) {
            return schema->parameters[i];
        }
    }

    for (size_t i = 0; i < schema->groups_len; i++) {
        return _internal_parameter_lookup(schema->groups[i], parameter_id);
    }

    return NULL;
}

/* to int path */
registry_namespace_int_path_t registry_to_namespace_int_path(const registry_namespace_t *namespace)
{
    assert(namespace != NULL);

    return (registry_namespace_int_path_t) {
               .namespace_id = namespace->id,
    };
}

registry_schema_int_path_t registry_to_schema_int_path(const registry_schema_t *schema)
{
    assert(schema != NULL);

    return (registry_schema_int_path_t) {
               .namespace_id = schema->namespace->id,
               .schema_id = schema->id,
    };
}

registry_instance_int_path_t registry_to_instance_int_path(const registry_instance_t *instance)
{
    assert(instance != NULL);

    return (registry_instance_int_path_t) {
               .namespace_id = instance->schema->namespace->id,
               .schema_id = instance->schema->id,
               .instance_id = instance->id,
    };
}

registry_group_int_path_t registry_to_group_int_path(const registry_instance_t *instance,
                                                     const registry_group_t *group)
{
    assert(instance != NULL);
    assert(group != NULL);

    return (registry_group_int_path_t) {
               .namespace_id = instance->schema->namespace->id,
               .schema_id = instance->schema->id,
               .instance_id = instance->id,
               .group_id = group->id,
    };
}

registry_parameter_int_path_t registry_to_parameter_int_path(const registry_instance_t *instance,
                                                             const registry_parameter_t *parameter)
{
    assert(instance != NULL);
    assert(parameter != NULL);

    return (registry_parameter_int_path_t) {
               .namespace_id = instance->schema->namespace->id,
               .schema_id = instance->schema->id,
               .instance_id = instance->id,
               .parameter_id = parameter->id,
    };
}

/* from int path */
int registry_from_namespace_int_path(const registry_namespace_int_path_t *path,
                                     registry_namespace_t **namespace)
{
    assert(path != NULL);

    if (namespace != NULL) {
        *namespace = (registry_namespace_t *)_namespace_lookup(path->namespace_id);
    }

    return 0;
}

int registry_from_schema_int_path(const registry_schema_int_path_t *path,
                                  registry_namespace_t **namespace, registry_schema_t **schema)
{
    assert(path != NULL);

    if (namespace != NULL) {
        *namespace = (registry_namespace_t *)_namespace_lookup(path->namespace_id);
    }

    if (schema != NULL) {
        *schema = (registry_schema_t *)_schema_lookup(*namespace, path->schema_id);
    }

    return 0;
}

int registry_from_instance_int_path(const registry_instance_int_path_t *path,
                                    registry_namespace_t **namespace, registry_schema_t **schema,
                                    registry_instance_t **instance)
{
    assert(path != NULL);

    if (namespace != NULL) {
        *namespace = (registry_namespace_t *)_namespace_lookup(path->namespace_id);
    }

    if (schema != NULL) {
        *schema = (registry_schema_t *)_schema_lookup(*namespace, path->schema_id);
    }

    if (instance != NULL) {
        *instance = (registry_instance_t *)_instance_lookup(*schema, path->instance_id);
    }

    return 0;
}

int registry_from_group_int_path(const registry_group_int_path_t *path,
                                 registry_namespace_t **namespace, registry_schema_t **schema,
                                 registry_instance_t **instance, registry_group_t **group)
{
    assert(path != NULL);

    if (namespace != NULL) {
        *namespace = (registry_namespace_t *)_namespace_lookup(path->namespace_id);
    }

    if (schema != NULL) {
        *schema = (registry_schema_t *)_schema_lookup(*namespace, path->schema_id);
    }

    if (instance != NULL) {
        *instance = (registry_instance_t *)_instance_lookup(*schema, path->instance_id);
    }

    if (group != NULL) {
        *group = (registry_group_t *)_group_lookup(*schema, path->group_id);
    }

    return 0;
}

int registry_from_parameter_int_path(const registry_parameter_int_path_t *path,
                                     registry_namespace_t **namespace, registry_schema_t **schema,
                                     registry_instance_t **instance,
                                     registry_parameter_t **parameter)
{
    assert(path != NULL);

    if (namespace != NULL) {
        *namespace = (registry_namespace_t *)_namespace_lookup(path->namespace_id);
    }

    if (schema != NULL) {
        *schema = (registry_schema_t *)_schema_lookup(*namespace, path->schema_id);
    }

    if (instance != NULL) {
        *instance = (registry_instance_t *)_instance_lookup(*schema, path->instance_id);
    }

    if (parameter != NULL) {
        *parameter = (registry_parameter_t *)_parameter_lookup(*schema, path->parameter_id);
    }

    return 0;
}
