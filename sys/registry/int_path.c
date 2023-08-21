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

XFA_USE_CONST(registry_namespace_t *, _registry_namespaces_xfa);

static int _namespace_lookup(const registry_namespace_id_t namespace_id,
                             registry_namespace_t **namespace)
{
    assert(namespace != NULL);

    for (size_t i = 0; i < XFA_LEN(registry_namespace_t *, _registry_namespaces_xfa); i++) {
        if (_registry_namespaces_xfa[i]->id == namespace_id) {
            *namespace = _registry_namespaces_xfa[i];
            return 0;
        }
    }

    return -REGISTRY_ERROR_NAMESPACE_NOT_FOUND;
}

static int _schema_lookup(const registry_namespace_t *namespace,
                          const registry_schema_id_t schema_id,
                          registry_schema_t **schema)
{
    assert(schema != NULL);

    for (size_t i = 0; i < namespace->schemas_len; i++) {
        if (namespace->schemas[i]->id == schema_id) {
            *schema = (registry_schema_t *)namespace->schemas[i];
            return 0;
        }
    }

    return -REGISTRY_ERROR_SCHEMA_NOT_FOUND;
}

static int _instance_lookup(const registry_schema_t *schema,
                            const registry_instance_id_t instance_id,
                            registry_instance_t **instance)
{
    assert(schema != NULL);
    assert(instance != NULL);

    /* find instance with correct instance_id */
    clist_node_t *node = schema->instances.next;

    if (!node) {
        return -REGISTRY_ERROR_INSTANCE_NOT_FOUND;
    }

    do {
        node = node->next;

        /* check if index equals instance_id */
        if (container_of(node, registry_instance_t, node)->id == instance_id) {
            *instance = container_of(node, registry_instance_t, node);
            return 0;
        }
    } while (node != schema->instances.next);

    return -REGISTRY_ERROR_INSTANCE_NOT_FOUND;
}

static const registry_group_t *_internal_group_lookup(const registry_group_t *group,
                                                      const registry_group_id_t group_id)
{
    assert(group != NULL);

    for (size_t i = 0; i < group->groups_len; i++) {
        if (group->groups[i]->id == group_id) {
            return group->groups[i];
        }
        else if (group->groups[i]->groups_len > 0) {
            return _internal_group_lookup(group->groups[i], group_id);
        }
    }

    return NULL;
}

static int _group_lookup(const registry_schema_t *schema, const registry_group_id_t group_id,
                         registry_group_t **group)
{
    assert(schema != NULL);
    assert(group != NULL);

    for (size_t i = 0; i < schema->groups_len; i++) {
        if (schema->groups[i]->id == group_id) {
            *group = (registry_group_t *)schema->groups[i];
            return 0;
        }
        else if (schema->groups[i]->groups_len > 0) {
            registry_group_t *found_group = (registry_group_t *)_internal_group_lookup(
                schema->groups[i], group_id);
            if (found_group != NULL) {
                *group = found_group;
                return 0;
            }
        }
    }

    return -REGISTRY_ERROR_GROUP_NOT_FOUND;
}

static const registry_parameter_t *_internal_parameter_lookup(const registry_group_t *group,
                                                              const registry_parameter_id_t parameter_id)
{
    assert(group != NULL);

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

static int _parameter_lookup(const registry_schema_t *schema,
                             const registry_parameter_id_t parameter_id,
                             registry_parameter_t **parameter)
{
    assert(schema != NULL);
    assert(parameter != NULL);

    for (size_t i = 0; i < schema->parameters_len; i++) {
        if (schema->parameters[i]->id == parameter_id) {
            *parameter = (registry_parameter_t *)schema->parameters[i];
            return 0;
        }
    }

    for (size_t i = 0; i < schema->groups_len; i++) {
        registry_parameter_t *found_parameter = (registry_parameter_t *)_internal_parameter_lookup(
            schema->groups[i],
            parameter_id);
        if (found_parameter != NULL) {
            *parameter = found_parameter;
            return 0;
        }
    }

    return -REGISTRY_ERROR_PARAMETER_NOT_FOUND;
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
    assert(namespace != NULL);

    int res = _namespace_lookup(path->namespace_id, namespace);

    return res;
}

int registry_from_schema_int_path(const registry_schema_int_path_t *path,
                                  registry_namespace_t **namespace, registry_schema_t **schema)
{
    assert(path != NULL);
    /* leaving namespace NULL is fine */
    assert(schema != NULL);

    registry_namespace_t *found_namespace;
    int res = _namespace_lookup(path->namespace_id, &found_namespace);
    if (namespace != NULL) {
        *namespace = found_namespace;
    }

    if (res == 0) {
        res = _schema_lookup(found_namespace, path->schema_id, schema);
    }

    return res;
}

int registry_from_instance_int_path(const registry_instance_int_path_t *path,
                                    registry_namespace_t **namespace, registry_schema_t **schema,
                                    registry_instance_t **instance)
{
    assert(path != NULL);
    /* leaving namespace or schema NULL is fine */
    assert(instance != NULL);

    registry_namespace_t *found_namespace;
    int res = _namespace_lookup(path->namespace_id, &found_namespace);
    if (namespace != NULL) {
        *namespace = found_namespace;
    }

    if (res == 0) {
        registry_schema_t *found_schema;
        res = _schema_lookup(found_namespace, path->schema_id, &found_schema);
        if (schema != NULL) {
            *schema = found_schema;
        }

        if (res == 0) {
            res = _instance_lookup(found_schema, path->instance_id, instance);
        }
    }

    return res;
}

int registry_from_group_int_path(const registry_group_int_path_t *path,
                                 registry_namespace_t **namespace, registry_schema_t **schema,
                                 registry_instance_t **instance, registry_group_t **group)
{
    assert(path != NULL);
    /* leaving namespace, schema or instance NULL is fine */
    assert(group != NULL);

    registry_namespace_t *found_namespace;
    int res = _namespace_lookup(path->namespace_id, &found_namespace);
    if (namespace != NULL) {
        *namespace = found_namespace;
    }

    if (res == 0) {
        registry_schema_t *found_schema;
        res = _schema_lookup(found_namespace, path->schema_id, &found_schema);
        if (schema != NULL) {
            *schema = found_schema;
        }

        if (res == 0) {
            registry_instance_t *found_instance;
            res = _instance_lookup(found_schema, path->instance_id, &found_instance);
            if (instance != NULL) {
                *instance = found_instance;
            }

            if (res == 0) {
                res = _group_lookup(found_schema, path->group_id, group);
            }
        }
    }

    return res;
}

int registry_from_parameter_int_path(const registry_parameter_int_path_t *path,
                                     registry_namespace_t **namespace, registry_schema_t **schema,
                                     registry_instance_t **instance,
                                     registry_parameter_t **parameter)
{
    assert(path != NULL);
    /* leaving namespace, schema or instance NULL is fine */
    assert(parameter != NULL);

    registry_namespace_t *found_namespace;
    int res = _namespace_lookup(path->namespace_id, &found_namespace);
    if (namespace != NULL) {
        *namespace = found_namespace;
    }

    if (res == 0) {
        registry_schema_t *found_schema;
        res = _schema_lookup(found_namespace, path->schema_id, &found_schema);
        if (schema != NULL) {
            *schema = found_schema;
        }

        if (res == 0) {
            registry_instance_t *found_instance;
            res = _instance_lookup(found_schema, path->instance_id, &found_instance);
            if (instance != NULL) {
                *instance = found_instance;
            }

            if (res == 0) {
                res = _parameter_lookup(found_schema, path->parameter_id, parameter);
            }
        }
    }

    return res;
}

int registry_from_group_or_parameter_int_path(const registry_group_or_parameter_int_path_t *path,
                                              registry_int_path_type_t *path_type,
                                              registry_namespace_t **namespace,
                                              registry_schema_t **schema,
                                              registry_instance_t **instance,
                                              registry_group_t **group,
                                              registry_parameter_t **parameter)
{
    assert(path != NULL);
    /* leaving namespace, schema or instance NULL is fine */
    assert(group != NULL);
    assert(parameter != NULL);

    registry_namespace_t *found_namespace;
    int res = _namespace_lookup(path->namespace_id, &found_namespace);
    if (namespace != NULL) {
        *namespace = found_namespace;
    }

    if (res == 0) {
        registry_schema_t *found_schema;
        res = _schema_lookup(found_namespace, path->schema_id, &found_schema);
        if (schema != NULL) {
            *schema = found_schema;
        }

        if (res == 0) {
            registry_instance_t *found_instance;
            res = _instance_lookup(found_schema, path->instance_id, &found_instance);
            if (instance != NULL) {
                *instance = found_instance;
            }

            if (res == 0) {
                res = _parameter_lookup(found_schema, path->group_or_parameter_id, parameter);
                if (res == 0) {
                    *path_type = REGISTRY_INT_PATH_TYPE_PARAMETER;
                }
                else {
                    res = _group_lookup(found_schema, path->group_or_parameter_id, group);
                    if (res == 0) {
                        *path_type = REGISTRY_INT_PATH_TYPE_GROUP;
                    }
                }
            }
        }
    }

    return res;
}
