/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_storage_heap RIOT Registry Path Storage: Heap
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

#include "registry/storage.h"
#include "registry/path.h"

#define STORE_CAPACITY 100

static int load(const registry_storage_instance_t *storage,
                const load_cb_t load_cb);
static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_resource_t *parameter,
                const registry_value_t *value);

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
} heap_storage_t;

/* This is the "storage device" containing all the data */
static heap_storage_t heap_storage[STORE_CAPACITY];
static size_t heap_storage_len = 0;

/* Storage interface descriptor to be registered in the RIOT Registry */
registry_storage_t registry_storage_heap = {
    .load = load,
    .save = save,
};

static int load(const registry_storage_instance_t *storage,
                const load_cb_t load_cb)
{
    (void)storage;

    for (size_t i = 0; i < heap_storage_len; i++) {
        const registry_value_t value = {
            .type = REGISTRY_TYPE_NONE,
            .buf = heap_storage[i].buf,
            .buf_len = heap_storage[i].buf_len,
        };

        const registry_path_t path = {
            .namespace_id = &heap_storage[i].namespace_id,
            .schema_id = &heap_storage[i].schema_id,
            .instance_id = &heap_storage[i].instance_id,
            .resource_id = &heap_storage[i].resource_id,
        };

        registry_value_t internal_value;
        registry_get_by_path(&path, &internal_value);

        load_cb(&internal_value, &value);
    }
    return 0;
}

static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_resource_t *parameter,
                const registry_value_t *value)
{
    (void)storage;
    // TODO finish this

    /* Search value in storage */
    for (size_t i = 0; i < heap_storage_len; i++) {
        if (heap_storage[i].namespace_id == parameter->schema->namespace.id &&
            heap_storage[i].schema_id == parameter->schema.id &&
            heap_storage[i].instance_id == instance_id &&
            heap_storage[i].resource_id == parameter->id) {
            memcpy(heap_storage[i].buf, value->buf, value->buf_len);
            return 0;
        }
    }

    /* Value not found in storage => Append it at the end */
    heap_storage[heap_storage_len].namespace_id = *parameter->schema->namespace.id;
    heap_storage[heap_storage_len].schema_id = *parameter->schema.id;
    heap_storage[heap_storage_len].instance_id = *instance_id;
    heap_storage[heap_storage_len].resource_id = *parameter->id;
    heap_storage[heap_storage_len].buf = malloc(value->buf_len);
    memcpy(heap_storage[heap_storage_len].buf, value->buf, value->buf_len);
    heap_storage[heap_storage_len].buf_len = value->buf_len;

    heap_storage_len++;
    return 0;
}
