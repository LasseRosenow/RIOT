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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "clist.h"
#include "registry.h"
#include "registry/util.h"
#include "registry/storage.h"

#include "registry/path.h"

static void _debug_print_path(const registry_path_t *path)
{
    if (ENABLE_DEBUG) {
        DEBUG("%d", *path->namespace_id);

        if (path->schema_id != NULL) {
            DEBUG("/%d", *path->schema_id);

            if (path->instance_id != NULL) {
                DEBUG("/%d", *path->instance_id);

                if (path->path_len > 0) {
                    DEBUG("/");

                    for (size_t i = 0; i < path->path_len; i++) {
                        DEBUG("%d", path->path[i]);

                        if (i < path->path_len - 1) {
                            DEBUG("/");
                        }
                    }
                }
            }
        }
    }
}

static const registry_schema_item_t *_parameter_lookup(const registry_path_t *path,
                                                       const registry_schema_t *schema)
{
    const registry_schema_item_t *schema_item;
    const registry_schema_item_t **schema_items = schema->items;
    size_t schema_items_len = schema->items_len;

    for (size_t path_index = 0; path_index < path->path_len; path_index++) {
        for (size_t i = 0; i < schema_items_len; i++) {
            schema_item = schema_items[i];

            if (schema_item->id == path->path[path_index]) {
                if (schema_item->type == REGISTRY_TYPE_GROUP) {
                    /* if this is not the last path segment and its a group => update schemas and schemas_len values */
                    schema_items = schema_item->items;
                    schema_items_len = schema_item->items_len;
                    break;
                }
                else if (path_index == path->path_len - 1) {
                    /* if this is the last path segment and it is a parameter => return the parameter */
                    return schema_item;
                }
            }
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
    const registry_schema_item_t *parameter = _parameter_lookup(path, schema);

    if (!parameter) {
        return -EINVAL;
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
    const registry_schema_item_t *parameter = _parameter_lookup(path, schema);

    if (!parameter) {
        return -EINVAL;
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
        for (size_t i = 0; i < namespace->items_len; i++) {
            const registry_schema_t *schema = namespace->items[i];
            registry_path_t new_path = {
                .namespace_id = path->namespace_id,
                .schema_id = &schema->id,
                .instance_id = NULL,
                .path = NULL,
                .path_len = 0,
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

        do {
            node = node->next;
            registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

            if (!namespace) {
                return -EINVAL;
            }

            /* create new path that includes the new namespace_id */
            registry_path_t new_path = {
                .namespace_id = &namespace->id,
                .schema_id = NULL,
                .instance_id = NULL,
                .path = NULL,
                .path_len = 0,
            };

            int _rc = _registry_commit_namespace_by_path(&new_path);

            if (!_rc) {
                rc = _rc;
            }
        } while (node != _registry_namespaces.next);
    }

    return rc;
}

static void _registry_export_params_by_path(const registry_path_export_cb_t export_cb,
                                            const registry_path_t *current_path,
                                            const registry_schema_item_t **schema_items,
                                            const size_t schema_items_len,
                                            const int recursion_depth, const void *context)
{
    for (size_t i = 0; i < schema_items_len; i++) {
        const registry_schema_item_t *schema_item = schema_items[i];

        /* create new path including the current schema_item */
        registry_id_t _new_path_path[current_path->path_len + 1];
        for (size_t j = 0; j < current_path->path_len; j++) {
            _new_path_path[j] = current_path->path[j];
        }
        _new_path_path[ARRAY_SIZE(_new_path_path) - 1] = schema_item->id;
        registry_path_t new_path = {
            .namespace_id = current_path->namespace_id,
            .schema_id = current_path->schema_id,
            .instance_id = current_path->instance_id,
            .path = _new_path_path,
            .path_len = ARRAY_SIZE(_new_path_path),
        };

        /* check if the current schema_item is a group or a parameter */
        if (schema_item->type == REGISTRY_TYPE_GROUP) {
            /* group => search for parameters */
            registry_export_cb_data_t export_data = { .group = schema_item };
            export_cb(&new_path, &export_data, REGISTRY_EXPORT_GROUP, NULL, context);

            /* if recursion_depth is not 1 then not only the group itself will be exported, but also its children depending on recursion_depth */
            if (recursion_depth != 1) {
                int new_recursion_depth = 0;     // create a new variable, because recursion_depth would otherwise be decreased in each cycle of the for loop
                if (recursion_depth != 0) {
                    new_recursion_depth = recursion_depth - 1;
                }

                _registry_export_params_by_path(export_cb, &new_path, schema_item->items,
                                                schema_item->items_len,
                                                new_recursion_depth, context);
            }
        }
        else {
            /* parameter found => export */
            registry_value_t value;
            registry_get_by_path(&new_path, &value);
            registry_export_cb_data_t export_data = { .parameter = schema_item };
            export_cb(&new_path, &export_data, REGISTRY_EXPORT_PARAMETER, &value, context);
        }
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
    if (path->path_len > 0) {
        const registry_schema_item_t *schema_item = _parameter_lookup(path, schema);

        /* create a new path which does not include the last value, because _registry_export_params_by_path will add it inside */
        registry_id_t _new_path_path[path->path_len - 1];
        for (size_t j = 0; j < path->path_len; j++) {
            _new_path_path[j] = path->path[j];
        }
        registry_path_t new_path = {
            .namespace_id = path->namespace_id,
            .schema_id = path->schema_id,
            .instance_id = path->instance_id,
            .path = _new_path_path,
            .path_len = ARRAY_SIZE(_new_path_path),
        };

        _registry_export_params_by_path(export_cb, &new_path, &schema_item, 1, recursion_depth,
                                        context);
    }
    /* schema/instance => export the schema instance meta data (name) and its parameters recursively depending on recursion_depth */
    else if (path->path_len == 0) {
        /* export instance parameters (recursion_depth == 1 means only the exact path, which would only be a specific instance in this case) */
        if (recursion_depth != 1) {
            int new_recursion_depth = 0;
            if (recursion_depth != 0) {
                new_recursion_depth = recursion_depth - 1;
            }

            _registry_export_params_by_path(export_cb, path, schema->items,
                                            schema->items_len, new_recursion_depth, context);
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
                    .path = NULL,
                    .path_len = 0,
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

            for (size_t i = 0; i < namespace->items_len; i++) {
                const registry_schema_t *schema = namespace->items[i];

                /* create new path that includes the new schema_id */
                registry_path_t new_path = {
                    .namespace_id = path->namespace_id,
                    .schema_id = &schema->id,
                    .instance_id = NULL,
                    .path = NULL,
                    .path_len = 0,
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
    for (size_t i = 0; i < path->path_len; i++) {
        DEBUG("/%d", path->path[i]);
    }
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

            do {
                node = node->next;
                registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

                if (!namespace) {
                    return -EINVAL;
                }

                /* create new path that includes the new namespace_id */
                registry_path_t new_path = {
                    .namespace_id = &namespace->id,
                    .schema_id = NULL,
                    .instance_id = NULL,
                    .path = NULL,
                    .path_len = 0,
                };

                int _rc = _registry_export_namespace_by_path(export_cb, &new_path,
                                                             new_recursion_depth,
                                                             context);

                if (!_rc) {
                    rc = _rc;
                }
            } while (node != _registry_namespaces.next);
        }
    }

    return rc;
}

/* registry_load */
static int _registry_load_by_path_cb(const registry_path_t *path,
                                     const registry_value_t *value,
                                     const void *context)
{
    (void)context;

    if (ENABLE_DEBUG) {
        DEBUG("[registry_storage] Loading: ");
        _debug_print_path(path);
        DEBUG(" = ");
        _debug_print_value(value);
        DEBUG("\n");
    }

    return registry_set_by_path(path, value);
}

int registry_load_by_path(const registry_path_t *path)
{
    clist_node_t *node = _storage_srcs.next;

    if (!node) {
        return -ENOENT;
    }

    do {
        node = node->next;
        registry_storage_instance_t *src = container_of(node, registry_storage_instance_t, node);
        src->itf->load(src, path, _registry_load_by_path_cb, NULL);
    } while (node != _storage_srcs.next);
    // TODO Possible bug? SFs could override with outdated values if SF_DST is not last in SF_SRCs?

    return 0;
}

/* registry_save */
static void _registry_storage_dup_check_cb(const registry_path_t *path,
                                           const registry_value_t *val,
                                           const void *cb_arg)
{
    assert(cb_arg != NULL);
    registry_dup_check_arg_t *dup_arg = (registry_dup_check_arg_t *)cb_arg;

    if (path->namespace_id != dup_arg->path->namespace_id ||
        path->schema_id != dup_arg->path->schema_id ||
        path->instance_id != dup_arg->path->instance_id) {
        return;
    }

    for (size_t i = 0; i < path->path_len; i++) {
        if (path->path[i] != dup_arg->path->path[i]) {
            return;
        }
    }

    if (memcmp(val->buf, dup_arg->val->buf, val->buf_len) == 0) {
        dup_arg->is_dup = true;
    }
}

static int _registry_save_by_path_export_cb(const registry_path_t *path,
                                            const registry_export_cb_data_t *data,
                                            const registry_export_cb_data_type_t data_type,
                                            const registry_value_t *value,
                                            const void *context)
{
    (void)context;
    (void)data;
    (void)data_type;
    (void)_registry_storage_dup_check_cb;

    /* The registry also exports just the namespace or just a schema, but the storage is only interested in paths with values */
    if (value == NULL) {
        return 0;
    }

    const registry_storage_instance_t *dst = _storage_dst;

    if (ENABLE_DEBUG) {
        DEBUG("[registry_storage] Saving: ");
        _debug_print_path(path);
        DEBUG(" = ");
        _debug_print_value(value);
        DEBUG("\n");
    }

    if (!dst) {
        return -ENOENT;
    }

    // TODO use registry_load_one() to remove overhead
    // registry_dup_check_arg_t dup = {
    //     .path = path,
    //     .val = *value,
    //     .is_dup = false,
    // };

    // _storage_dst->itf->load(_storage_dst, _registry_storage_dup_check_cb, &dup);

    // if (dup.is_dup) {
    //     return -EEXIST;
    // }

    /* only parameters need to be exported to storage, not namespaces, schemas or groups*/
    if (value != NULL) {
        return dst->itf->save(dst, path, *value);
    }

    return 0;
}

int registry_save_by_path(const registry_path_t *path)
{
    int res;

    if (!_storage_dst) {
        return -ENOENT;
    }

    if (_storage_dst->itf->save_start) {
        _storage_dst->itf->save_start(_storage_dst);
    }

    res = registry_export_by_path(_registry_save_by_path_export_cb, path, 0, NULL);

    if (_storage_dst->itf->save_end) {
        _storage_dst->itf->save_end(_storage_dst);
    }

    return res;
}

int registry_path_util_parse_string_path(const char *string_path,
                                         registry_path_t *registry_path,
                                         registry_id_t *path_items_buf)
{
    char *ptr = (char *)string_path;

    int i = 0;

    registry_path->path_len = 0;

    while (*ptr != '\0') {
        registry_id_t id = strtol(ptr, &ptr, 10);

        switch (i) {
        case 0: *(registry_id_t *)registry_path->namespace_id = id; break;
        case 1: *(registry_id_t *)registry_path->schema_id = id; break;
        case 2: *(registry_id_t *)registry_path->instance_id = id; break;
        default:
            path_items_buf[i] = id;
            registry_path->path_len++;
            break;
        }

        registry_path->path = path_items_buf;

        if (*ptr != '\0') {
            ptr++;
        }

        i++;
    }

    return 0;
}
