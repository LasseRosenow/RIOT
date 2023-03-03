/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_storage_facility_heap RIOT Registry Storage Facilities: Heap
 * @ingroup     sys
 * @brief       RIOT Registry Heap Storage Facility, only uses the heap for testing.
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

/* The storage_facility argument is the descriptor of the storage facility */
static int load(const registry_storage_facility_instance_t *instance, const storage_path_t path,
                const load_cb_t cb, const void *cb_arg);
static int save(const registry_storage_facility_instance_t *instance, const storage_path_t path,
                const registry_value_t value);

/*
   This conceptual example shows the implementation of a Storage Facility.
   The storage device is a storage array (represented by the
   `storage_facility_storage_t` struct).
 */
typedef struct {
    registry_namespace_id_t namespace_id;
    registry_id_t schema_id;
    registry_id_t instance_id;
    registry_id_t path[REGISTRY_MAX_DIR_DEPTH];
    size_t path_len;
    void *buf;
    size_t buf_len;
} storage_facility_storage_t;

/* This will be our "storage device \
    " */
static storage_facility_storage_t storage_facility[STORE_CAPACITY];

/* Storage Facility interface descriptor to be registered in the RIOT
   Registry */
registry_storage_facility_t registry_storage_facility_heap = {
    .load = load,
    .save = save,
};

/* Implementation of `load`. Execute a `cb` callback for each configuration
   found in the storage array */
static int load(const registry_storage_facility_instance_t *instance, const storage_path_t path,
                const load_cb_t cb, const void *cb_arg)
{
    // TODO implement "path        \
    //     " parameter!!
    (void)instance;
    (void)path;
    storage_path_t new_path;

    for (size_t i = 0; i < STORE_CAPACITY; i++) {
        if (storage_facility[i].path_len > 0) {
            new_path = (storage_path_t) {
                .namespace_id = &storage_facility[i].namespace_id,
                .schema_id = &storage_facility[i].schema_id,
                .instance_id = &storage_facility[i].instance_id,
                .path = storage_facility[i].path,
                .path_len = storage_facility[i].path_len,
            };

            registry_value_t value = {
                .type = REGISTRY_TYPE_NONE,
                .buf = storage_facility[i].buf,
                .buf_len = storage_facility[i].buf_len,
            };

            cb(new_path, value, cb_arg);
        }
    }
    return 0;
}

/* Implementation of `storage_facility`. Save parameter with given name and value in
   the storage array */
static int save(const registry_storage_facility_instance_t *instance, const storage_path_t path,
                const registry_value_t value)
{
    int free_slot = -1;

    (void)instance;

    if (path.schema_id == NULL) {
        return -EINVAL;
    }

    if (path.instance_id == NULL) {
        return -EINVAL;
    }

    for (size_t i = 0; i < STORE_CAPACITY; i++) {
        if (storage_facility[i].path_len > 0) {
            if (storage_facility[i].path_len == path.path_len) {
                for (size_t j = 0; j < storage_facility[i].path_len; j++) {
                    if (storage_facility[i].path[j] != path.path[j]) {
                        break;
                    }
                    else if (j == storage_facility[i].path_len - 1) {
                        // All ids within the path matched and this was the last one.
                        memcpy(storage_facility[i].buf, value.buf, value.buf_len);
                        return 0;
                    }
                }
            }
        }
        else {
            if (free_slot == -1) {
                free_slot = i;
            }
        }
    }

    if (free_slot == -1) {
        return -1;
    }

    storage_facility[free_slot].namespace_id = *path.namespace_id;
    storage_facility[free_slot].schema_id = *path.schema_id;
    storage_facility[free_slot].instance_id = *path.instance_id;
    memcpy(storage_facility[free_slot].path, path.path, path.path_len * sizeof(int));
    storage_facility[free_slot].path_len = path.path_len;
    storage_facility[free_slot].buf = malloc(value.buf_len);
    memcpy(storage_facility[free_slot].buf, value.buf, value.buf_len);
    storage_facility[free_slot].buf_len = value.buf_len;
    return 0;
}
