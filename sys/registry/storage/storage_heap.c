/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_storage_heap RIOT Registry Storage: Heap
 * @ingroup     sys
 * @brief       RIOT Registry Storage Heap, only uses the heap for testing.
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

#define STORE_CAPACITY 100

static int load(const registry_storage_instance_t *storage,
                const load_cb_t load_cb);
static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_parameter_t *parameter,
                const registry_value_t *value);

typedef struct {
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;
    registry_parameter_t *parameter;
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
        load_cb(heap_storage[i].instance, heap_storage[i].parameter, heap_storage[i].buf,
                heap_storage[i].buf_len);
    }
    return 0;
}

static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_parameter_t *parameter,
                const registry_value_t *value)
{
    (void)storage;

    /* Search value in storage */
    for (size_t i = 0; i < heap_storage_len; i++) {
        if (heap_storage[i].namespace == parameter->schema->namespace &&
            heap_storage[i].schema == parameter->schema &&
            heap_storage[i].instance == instance &&
            heap_storage[i].parameter == parameter) {
            memcpy(heap_storage[i].buf, value->buf, value->buf_len);
            return 0;
        }
    }

    /* Value not found in storage => Append it at the end */
    heap_storage[heap_storage_len].namespace = parameter->schema->namespace;
    heap_storage[heap_storage_len].schema = parameter->schema;
    heap_storage[heap_storage_len].instance = instance;
    heap_storage[heap_storage_len].parameter = parameter;
    heap_storage[heap_storage_len].buf = malloc(value->buf_len);
    memcpy(heap_storage[heap_storage_len].buf, value->buf, value->buf_len);
    heap_storage[heap_storage_len].buf_len = value->buf_len;

    heap_storage_len++;
    return 0;
}
