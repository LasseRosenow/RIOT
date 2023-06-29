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

#include "registry/path.h"

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

static const registry_resource_t *_internal_resource_lookup(const registry_resource_t **resources,
                                                            size_t resources_len,
                                                            const registry_resource_id_t resource_id)
{
    const registry_resource_t *resource;

    for (size_t i = 0; i < resources_len; i++) {
        resource = resources[i];

        if (resource->id == resource_id) {
            return resource;
        }
        else if (resource->type == REGISTRY_TYPE_GROUP) {
            return _internal_resource_lookup(resource->resources, resource->resources_len,
                                             resource_id);
        }
    }

    return NULL;
}

static const registry_resource_t *_resource_lookup(const registry_schema_t *schema,
                                                   const registry_resource_id_t resource_id)
{
    const registry_resource_t *resource;
    const registry_resource_t **resources = schema->resources;
    size_t resources_len = schema->resources_len;

    for (size_t i = 0; i < resources_len; i++) {
        resource = resources[i];

        if (resource->id == resource_id) {
            return resource;
        }
        else if (resource->type == REGISTRY_TYPE_GROUP) {
            return _internal_resource_lookup(resource->resources, resource->resources_len,
                                             resource_id);
        }
    }

    return NULL;
}

/* convert to path */
registry_namespace_path_t registry_path_from_namespace(const registry_namespace_t *namespace)
{
    assert(namespace != NULL);

    return (registry_namespace_path_t) {
               .namespace_id = namespace->id,
    };
}

registry_schema_path_t registry_path_from_schema(const registry_schema_t *schema)
{
    assert(schema != NULL);

    return (registry_schema_path_t) {
               .namespace_id = schema->namespace->id,
               .schema_id = schema->id,
    };
}

registry_instance_path_t registry_path_from_instance(const registry_instance_t *instance)
{
    assert(instance != NULL);

    return (registry_instance_path_t) {
               .namespace_id = instance->schema->namespace->id,
               .schema_id = instance->schema->id,
               .instance_id = instance->id,
    };
}

registry_resource_path_t registry_path_from_resource(const registry_instance_t *instance,
                                                     const registry_resource_t *resource)
{
    assert(instance != NULL);
    assert(resource != NULL);

    return (registry_resource_path_t) {
               .namespace_id = instance->schema->namespace->id,
               .schema_id = instance->schema->id,
               .instance_id = instance->id,
               .resource_id = resource->id,
    };
}

/* convert from path */
int registry_from_namespace_path(const registry_namespace_path_t *path,
                                 registry_namespace_t **namespace)
{
    assert(path != NULL);

    *namespace = _namespace_lookup(path->namespace_id);

    return 0;
}

int registry_from_schema_path(const registry_schema_path_t *path,
                              registry_namespace_t **namespace, registry_schema_t **schema)
{
    assert(path != NULL);

    *namespace = _namespace_lookup(path->namespace_id);
    *schema = _schema_lookup(*namespace, path->schema_id);

    return 0;
}

int registry_from_instance_path(const registry_instance_path_t *path,
                                registry_namespace_t **namespace, registry_schema_t **schema,
                                registry_instance_t **instance)
{
    assert(path != NULL);

    *namespace = _namespace_lookup(path->namespace_id);
    *schema = _schema_lookup(*namespace, path->schema_id);
    *instance = _instance_lookup(*schema, path->instance_id);

    return 0;
}

int registry_from_resource_path(const registry_resource_path_t *path,
                                registry_namespace_t **namespace, registry_schema_t **schema,
                                registry_instance_t **instance, registry_resource_t **resource)
{
    assert(path != NULL);

    *namespace = _namespace_lookup(path->namespace_id);
    *schema = _schema_lookup(*namespace, path->schema_id);
    *instance = _instance_lookup(*schema, path->instance_id);
    *resource = _resource_lookup(*schema, path->resource_id);

    return 0;
}
