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

XFA_INIT_CONST(registry_storage_instance_t *, _registry_storage_instances_src_xfa);

/* registry_load */
static registry_error_t _registry_load_cb(const registry_node_t *node, const void *buf, const size_t buf_len)
{
    assert(node->type == REGISTRY_NODE_PARAMETER);
    assert(node->value.parameter.parameter != NULL);
    assert(node->value.parameter.instance != NULL);

    return registry_set(node, buf, buf_len);
}

registry_error_t registry_load(void)
{
    for (size_t i = 0;
         i < XFA_LEN(registry_storage_instance_t *, _registry_storage_instances_src_xfa); i++) {
        registry_storage_instance_t *src = _registry_storage_instances_src_xfa[i];

        registry_error_t res = src->storage->load(src, _registry_load_cb);

        if (res != 0) {
            return res;
        }
    }

    return REGISTRY_ERROR_NONE;
}

/* registry_save */
static registry_error_t _registry_save_export_cb(const registry_node_t *node, const void *context)
{
    (void)context;

    /* the registry also exports just the namespace or just a schema, but the storage is only interested in configuration parameter values */
    if (node->type != REGISTRY_NODE_PARAMETER) {
        return REGISTRY_ERROR_NONE;
    }

    /* check if a destination storage is registered */
    if (!_registry_storage_instance_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
    }

    /* get value of configuration parameter */
    registry_value_t value;
    registry_get(node, &value);

    /* save parameter value via the save function of the registered destination storage */
    return _registry_storage_instance_dst->storage->save(_registry_storage_instance_dst,
                                                         node, &value);
}

registry_error_t registry_save(const registry_node_t *node)
{
    registry_error_t res;

    if (!_registry_storage_instance_dst) {
        return -REGISTRY_ERROR_NO_DST_STORAGE;
    }

    if (_registry_storage_instance_dst->storage->save_start) {
        _registry_storage_instance_dst->storage->save_start(_registry_storage_instance_dst);
    }
    
    res = registry_export(node, _registry_save_export_cb, 0, NULL);

    if (_registry_storage_instance_dst->storage->save_end) {
        _registry_storage_instance_dst->storage->save_end(_registry_storage_instance_dst);
    }

    return res;
}
