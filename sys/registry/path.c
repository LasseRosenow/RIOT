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

    registry_namespace_id_t index = 0;

    do {
        node = node->next;
        const registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

        if (index == namespace_id) {
            return namespace;
        }

        index++;
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

    registry_instance_id_t index = 0;

    do {
        node = node->next;
        const registry_instance_t *instance = container_of(node, registry_instance_t, node);

        /* check if index equals instance_id */
        if (index == instance_id) {
            return instance;
        }

        index++;
    } while (node != schema->instances.next);

    return NULL;
}

static const registry_resource_t *_resource_lookup(const registry_schema_t *schema,
                                                   const registry_resource_id_t resource_id)
{
    // TODO this is incorrect. this function does not find nested resources
    const registry_resource_t *schema_resource;
    const registry_resource_t **schema_resources = schema->resources;
    size_t schema_resources_len = schema->resources_len;

    for (size_t i = 0; i < schema_resources_len; i++) {
        schema_resource = schema_resources[i];

        if (schema_resource->id == resource_id) {
            return schema_resource;
        }
    }

    return NULL;
}

/* convert to path */
int registry_path_from_namespace(const registry_namespace_t *namespace, registry_path_t *path,
                                 registry_namespace_id_t *namespace_id_buf)
{
    assert(namespace != NULL);
    assert(path != NULL);

    clist_node_t *node = _registry_namespaces.next;

    if (!node) {
        return -EINVAL;
    }

    do {
        node = node->next;

        if (namespace == container_of(node, registry_namespace_t, node)) {
            path->namespace_id = namespace_id_buf;
            break;
        }

        (*namespace_id_buf)++;
    } while (node != _registry_namespaces.next);

    return 0;
}

int registry_path_from_schema(const registry_schema_t *schema, registry_path_t *path,
                              registry_namespace_id_t *namespace_id_buf)
{
    assert(schema != NULL);
    assert(path != NULL);

    int result = 0;

    /* set namespace_id */
    result = registry_path_from_namespace(schema->namespace, path, namespace_id_buf);

    /* set schema_id */
    path->schema_id = &schema->id;

    return result;
}

int registry_path_from_instance(const registry_instance_t *instance, registry_path_t *path,
                                registry_namespace_id_t *namespace_id_buf,
                                registry_instance_id_t *instance_id_buf)
{
    assert(instance != NULL);
    assert(path != NULL);

    int result = 0;

    /* set namespace_id and schema_id */
    result = registry_path_from_schema(instance->schema, path, namespace_id_buf);

    /* set instance_id */
    clist_node_t *node = instance->schema->instances.next;

    if (!node) {
        return -EINVAL;
    }

    do {
        node = node->next;

        if (instance == container_of(node, registry_instance_t, node)) {
            path->instance_id = instance_id_buf;
            break;
        }

        (*instance_id_buf)++;
    } while (node != _registry_namespaces.next);

    return result;
}

int registry_path_from_resource(const registry_instance_t *instance,
                                const registry_resource_t *resource, registry_path_t *path,
                                registry_namespace_id_t *namespace_id_buf,
                                registry_instance_id_t *instance_id_buf)
{
    assert(instance != NULL);
    assert(resource != NULL);
    assert(path != NULL);

    int result = 0;

    /* set namespace_id, schema_id and instance_id */
    result = registry_path_from_instance(instance, path, namespace_id_buf, instance_id_buf);

    /* set resource_id */
    path->resource_id = &resource->id;

    return result;
}

/* convert from path */
registry_namespace_t *registry_namespace_from_path(const registry_path_t *path)
{
    assert(path != NULL);

    /* lookup namespace */
    return registry_util_namespace_lookup(*path->namespace_id);
}

registry_schema_t *registry_schema_from_path(const registry_path_t *path)
{
    assert(path != NULL);

    /* lookup namespace */
    const registry_namespace_t *namespace = registry_util_namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return NULL;
    }

    /* lookup schema */
    return registry_util_schema_lookup(namespace, *path->schema_id);
}

registry_instance_t *registry_instance_from_path(const registry_path_t *path)
{
    assert(path != NULL);

    /* lookup namespace */
    const registry_namespace_t *namespace = registry_util_namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return NULL;
    }

    /* lookup schema */
    const registry_schema_t *schema = registry_util_schema_lookup(namespace, *path->schema_id);

    if (!schema) {
        return NULL;
    }

    /* lookup instance */
    return _instance_lookup(schema, *path->instance_id);
}

registry_resource_t *registry_resource_from_path(const registry_path_t *path)
{
    assert(path != NULL);

    /* lookup namespace */
    const registry_namespace_t *namespace = _namespace_lookup(*path->namespace_id);

    if (!namespace) {
        return NULL;
    }

    /* lookup schema */
    const registry_schema_t *schema = _schema_lookup(namespace, *path->schema_id);

    if (!schema) {
        return NULL;
    }

    /* lookup instance */
    const registry_instance_t *instance = _instance_lookup(schema, *path->instance_id);

    if (!instance) {
        return NULL;
    }

    /* lookup resource */
    return _resource_lookup(path, schema);
}

/* util */
// TODO this cannot work, the registry_path struct only has pointers.
// If we use stack variables to fill the pointer, then the value will be lost after the function is left
int registry_path_from_string(const char *string_path,
                              registry_path_t *path)
{
    char *ptr = (char *)string_path;

    int i = 0;

    while (*ptr != '\0') {
        registry_id_t id = strtol(ptr, &ptr, 10);

        switch (i) {
        case 0: *(registry_namespace_id_t *)path->namespace_id = id; break;
        case 1: *(registry_schema_id_t *)path->schema_id = id; break;
        case 2: *(registry_instance_id_t *)path->instance_id = id; break;
        case 3: *(registry_resource_id_t *)path->resource_id = id; break;
        }

        if (*ptr != '\0') {
            ptr++;
        }

        i++;
    }

    return 0;
}
