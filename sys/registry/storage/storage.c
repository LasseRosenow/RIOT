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
#include <string.h>
#include <assert.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "container.h"

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

/* registry_load */
static int _registry_load_cb(const registry_value_t *internal_value,
                             const registry_value_t *value)
{
    memcpy(internal_value->buf, value->buf, internal_value->buf_len);

    return 0;
}

int registry_load(void)
{
    clist_node_t *node = _storage_srcs.next;

    if (!node) {
        return -ENOENT;
    }

    do {
        node = node->next;
        registry_storage_instance_t *src = container_of(node, registry_storage_instance_t,
                                                        node);
        src->itf->load(src, _registry_load_cb);
    } while (node != _storage_srcs.next);
    // TODO Possible bug? SFs could override with outdated values if SF_DST is not last in SF_SRCs?

    return 0;
}

/* registry_save */
static int _registry_save_export_cb(const registry_export_cb_data_t *data,
                                    const registry_export_cb_data_type_t data_type,
                                    const void *context)
{
    (void)context;
    (void)data;
    (void)data_type;

    /* The registry also exports just the namespace or just a schema, but the storage is only interested in configuration parameter values */
    if (data_type != REGISTRY_EXPORT_PARAMETER) {
        return 0;
    }

    // TODO export must export all the parameter values or the different instances, for example by providing the instance as a field of parameter->data?

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

    /* only parameters need to be exported to storage, not namespaces, schemas or groups */
    if (value != NULL) {
        return dst->itf->save(dst, path, value);
    }

    return 0;
}

int registry_save(void)
{
    int res;

    if (!_storage_dst) {
        return -ENOENT;
    }

    if (_storage_dst->itf->save_start) {
        _storage_dst->itf->save_start(_storage_dst);
    }

    res = registry_export(_registry_save_export_cb, 0, NULL);

    if (_storage_dst->itf->save_end) {
        _storage_dst->itf->save_end(_storage_dst);
    }

    return res;
}

int registry_save_namespace(const registry_namespace_t *namespace);

int registry_save_schema(const registry_schema_t *schema);

int registry_save_instance(const registry_instance_t *instance);

int registry_save_group(const registry_instance_t *instance, const registry_resource_t *group);

int registry_save_parameter(const registry_instance_t *instance,
                            const registry_resource_t *parameter);
