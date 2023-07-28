/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_tests_full RIOT Registry Schema: Full
 * @ingroup     tests
 * @brief       RIOT Registry Full Schema representing all possible data types of the riot registry
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "registry.h"
#include "namespace_tests.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_SYS_RGB_LED) || IS_ACTIVE(DOXYGEN)

/* Mapping */
static void mapping(const registry_resource_id_t parameter_id, const registry_instance_t *instance,
                    void **val, size_t *val_len)
{
    registry_tests_full_instance_t *_instance =
        (registry_tests_full_instance_t *)instance->data;

    switch (parameter_id) {
    case registry_tests_full_RED:
        *val = &_instance->red;
        *val_len = sizeof(_instance->red);
        break;

    case registry_tests_full_GREEN:
        *val = &_instance->green;
        *val_len = sizeof(_instance->green);
        break;

    case registry_tests_full_BLUE:
        *val = &_instance->blue;
        *val_len = sizeof(_instance->blue);
        break;

    case registry_tests_full_BRIGHTNESSES_WHITE:
        *val = &_instance->white;
        *val_len = sizeof(_instance->white);
        break;

    case registry_tests_full_BRIGHTNESSES_YELLOW:
        *val = &_instance->yellow;
        *val_len = sizeof(_instance->yellow);
        break;
    }
}

/* Schema */
const registry_resource_t registry_tests_full_string = {
    .id = 0,
    .name = "string",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_STRING,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_full_boolean = {
    .id = 1,
    .name = "boolean",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_BOOLEAN,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_full_u8 = {
    .id = 2,
    .name = "u8",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_UINT8,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_full_u16 = {
    .id = 3,
    .name = "u16",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_UINT16,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_full_u32 = {
    .id = 4,
    .name = "u32",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_UINT32,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_u64 = {
    .id = 5,
    .name = "u64",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_UINT64,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_full_i8 = {
    .id = 2,
    .name = "i8",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_INT8,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_full_i16 = {
    .id = 3,
    .name = "i16",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_INT16,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_full_i32 = {
    .id = 4,
    .name = "i32",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_INT32,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_i64 = {
    .id = 5,
    .name = "i64",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_INT64,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_full_f32 = {
    .id = 4,
    .name = "f32",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_FLOAT32,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_tests_f64 = {
    .id = 5,
    .name = "f64",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_FLOAT64,
    .resources = NULL,
    .resources_len = 0,
};

registry_schema_t registry_tests_full = {
    .id = 0,
    .name = "full",
    .description = "",
    .namespace = &registry_tests,
    .mapping = mapping,
    .resources = (const registry_resource_t *[]) {
        &registry_tests_full_string,
        &registry_tests_full_boolean,
        &registry_tests_full_u8,
        &registry_tests_full_u16,
        &registry_tests_full_u32,
        &registry_tests_full_u64,
        &registry_tests_full_i8,
        &registry_tests_full_i16,
        &registry_tests_full_i32,
        &registry_tests_full_i64,
        &registry_tests_full_f32,
        &registry_tests_full_f64,
    },
    .resources_len = 12,
};

#endif
