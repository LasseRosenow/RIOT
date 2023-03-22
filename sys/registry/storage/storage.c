/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_storage RIOT Registry Storage
 * @ingroup     sys
 * @brief       RIOT Registry Storage module allowing to store configuration parameters to non-volatile storage
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define ENABLE_DEBUG (0)
#include "debug.h"

#include "registry/storage.h"
#include "registry/util.h"

const registry_storage_instance_t *_storage_dst;
clist_node_t _storage_srcs;

void registry_register_storage_src(const registry_storage_instance_t *src)
{
    assert(src != NULL);
    clist_rpush((clist_node_t *)&_storage_srcs, (clist_node_t *)&(src->node));
}

void registry_register_storage_dst(const registry_storage_instance_t *dst)
{
    assert(dst != NULL);
    _storage_dst = dst;
}

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
        src->itf->operations.path.load(src, path, _registry_load_by_path_cb);
    } while (node != _storage_srcs.next);
    // TODO Possible bug? SFs could override with outdated values if SF_DST is not last in SF_SRCs?

    return 0;
}

/* registry_save */
static void _registry_storage_dup_check_cb(const registry_path_t *path,
                                           const registry_value_t *val,
                                           const void *context)
{
    assert(context != NULL);
    registry_dup_check_arg_t *dup_arg = (registry_dup_check_arg_t *)context;

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
        return dst->itf->operations.path.save(dst, path, value);
    }

    return 0;
}

int registry_save_by_path(const registry_path_t *path)
{
    int res;

    if (!_storage_dst) {
        return -ENOENT;
    }

    if (_storage_dst->itf->operations.path.save_start) {
        _storage_dst->itf->operations.path.save_start(_storage_dst);
    }

    res = registry_export_by_path(_registry_save_by_path_export_cb, path, 0, NULL);

    if (_storage_dst->itf->operations.path.save_end) {
        _storage_dst->itf->operations.path.save_end(_storage_dst);
    }

    return res;
}
