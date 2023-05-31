/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_path_storage_heap RIOT Registry Path Storage: Heap
 * @ingroup     sys
 * @brief       RIOT Registry Path Storage Heap, only uses the heap for testing.
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"

#include "registry/path/storage.h"
#include "registry/path.h"

#define STORE_CAPACITY 100

static int load(const registry_path_storage_instance_t *instance, const registry_path_t *path,
                const load_by_path_cb_t cb, const void *cb_arg);
static int save(const registry_path_storage_instance_t *instance, const registry_path_t *path,
                const registry_value_t value);

/*
   This conceptual example shows the implementation of a storage.
   The storage device is a storage array (represented by the
   `storage_storage_t` struct).
 */
typedef struct {
    registry_id_t namespace_id;
    registry_id_t schema_id;
    registry_id_t instance_id;
    registry_id_t resource_id;
    void *buf;
    size_t buf_len;
} storage_storage_t;

/* This is the "storage device" containing all the data */
static storage_storage_t storage[STORE_CAPACITY];
static size_t storage_len = 0;

/* Storage interface descriptor to be registered in the RIOT Registry */
registry_path_storage_t registry_path_storage_heap = {
    .load = load,
    .save = save,
};

static int load(const registry_path_storage_instance_t *instance, const registry_path_t *path,
                const load_by_path_cb_t cb, const void *cb_arg)
{
    // TODO implement "path" parameter!!
    (void)instance;
    (void)path;

    for (size_t i = 0; i < storage_len; i++) {
        const registry_path_t new_path = {
            .namespace_id = &storage[i].namespace_id,
            .schema_id = &storage[i].schema_id,
            .instance_id = &storage[i].instance_id,
            .resource_id = &storage[i].resource_id,
        };

        const registry_value_t value = {
            .type = REGISTRY_TYPE_NONE,
            .buf = storage[i].buf,
            .buf_len = storage[i].buf_len,
        };

        cb(&new_path, &value, cb_arg);
    }
    return 0;
}

static int save(const registry_path_storage_instance_t *instance, const registry_path_t *path,
                const registry_value_t value)
{
    (void)instance;

    if (path->namespace_id == NULL) {
        return -EINVAL;
    }

    if (path->schema_id == NULL) {
        return -EINVAL;
    }

    if (path->instance_id == NULL) {
        return -EINVAL;
    }

    if (path->resource_id == NULL) {
        return -EINVAL;
    }

    /* Search value in storage */
    for (size_t i = 0; i < storage_len; i++) {
        if (storage[i].namespace_id == path->namespace_id &&
            storage[i].schema_id == path->schema_id &&
            storage[i].instance_id == path->instance_id &&
            storage[i].resource_id == path->resource_id) {
            memcpy(storage[i].buf, value.buf, value.buf_len);
            return 0;
        }
    }

    /* Value not found in storage => Append it at the end */
    storage[storage_len].namespace_id = *path->namespace_id;
    storage[storage_len].schema_id = *path->schema_id;
    storage[storage_len].instance_id = *path->instance_id;
    storage[storage_len].resource_id = *path->resource_id;
    storage[storage_len].buf = malloc(value.buf_len);
    memcpy(storage[storage_len].buf, value.buf, value.buf_len);
    storage[storage_len].buf_len = value.buf_len;

    storage_len++;
    return 0;
}
