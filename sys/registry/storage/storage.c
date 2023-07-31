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
#include "kernel_defines.h"
#include "clist.h"

#include "registry/storage.h"
#include "registry/util.h"
#include "registry/error.h"

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
static int _registry_load_cb(const registry_instance_t *instance,
                             const registry_resource_t *parameter,
                             const void *buf, const size_t buf_len)
{
    return registry_set(instance, parameter, &buf, buf_len);
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

    return 0;
}

/* registry_save */
static int _registry_save_export_cb(const registry_export_cb_data_t *data,
                                    const registry_export_cb_data_type_t data_type,
                                    const void *context)
{
    (void)context;

    /* the registry also exports just the namespace or just a schema, but the storage is only interested in configuration parameter values */
    if (data_type != REGISTRY_EXPORT_PARAMETER) {
        return 0;
    }

    /* check if a destination storage is registered */
    if (!_storage_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
    }

    /* get value of configuration parameter */
    registry_value_t value;
    registry_get(data->parameter.instance, data->parameter.data, &value);

    /* save parameter value via the save function of the registered destination storage */
    return _storage_dst->itf->save(_storage_dst, data->parameter.instance, data->parameter.data,
                                   &value);
}

int registry_save(void)
{
    int res;

    if (!_storage_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
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

int registry_save_namespace(const registry_namespace_t *namespace)
{
    int res;

    if (!_storage_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
    }

    if (_storage_dst->itf->save_start) {
        _storage_dst->itf->save_start(_storage_dst);
    }

    res = registry_export_namespace(namespace, _registry_save_export_cb, 0, NULL);

    if (_storage_dst->itf->save_end) {
        _storage_dst->itf->save_end(_storage_dst);
    }

    return res;
}

int registry_save_schema(const registry_schema_t *schema)
{
    int res;

    if (!_storage_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
    }

    if (_storage_dst->itf->save_start) {
        _storage_dst->itf->save_start(_storage_dst);
    }

    res = registry_export_schema(schema, _registry_save_export_cb, 0, NULL);

    if (_storage_dst->itf->save_end) {
        _storage_dst->itf->save_end(_storage_dst);
    }

    return res;
}

int registry_save_instance(const registry_instance_t *instance)
{
    int res;

    if (!_storage_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
    }

    if (_storage_dst->itf->save_start) {
        _storage_dst->itf->save_start(_storage_dst);
    }

    res = registry_export_instance(instance, _registry_save_export_cb, 0, NULL);

    if (_storage_dst->itf->save_end) {
        _storage_dst->itf->save_end(_storage_dst);
    }

    return res;
}

int registry_save_group(const registry_instance_t *instance, const registry_resource_t *group)
{
    int res;

    if (!_storage_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
    }

    if (_storage_dst->itf->save_start) {
        _storage_dst->itf->save_start(_storage_dst);
    }

    res = registry_export_group(instance, group, _registry_save_export_cb, 0, NULL);

    if (_storage_dst->itf->save_end) {
        _storage_dst->itf->save_end(_storage_dst);
    }

    return res;
}

int registry_save_parameter(const registry_instance_t *instance,
                            const registry_resource_t *parameter)
{
    int res;

    if (!_storage_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
    }

    if (_storage_dst->itf->save_start) {
        _storage_dst->itf->save_start(_storage_dst);
    }

    res = registry_export_parameter(instance, parameter, _registry_save_export_cb, NULL);

    if (_storage_dst->itf->save_end) {
        _storage_dst->itf->save_end(_storage_dst);
    }

    return res;
}
