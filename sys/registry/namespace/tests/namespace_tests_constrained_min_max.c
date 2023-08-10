/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_tests_constrained_min_max RIOT Registry Schema: Constrained Min Max
 * @ingroup     sys
 * @brief       RIOT Registry Constrained Min Max Schema representing all possible data types of the riot registry with minimum and maximum value constraints
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
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/constrained_min_max.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_CONSTRAINED_MIN_MAX) || IS_ACTIVE(DOXYGEN)

/* Mapping */
static void mapping(const registry_parameter_id_t parameter_id, const registry_instance_t *instance,
                    void **val, size_t *val_len)
{
    registry_tests_constrained_min_max_instance_t *_instance =
        (registry_tests_constrained_min_max_instance_t *)instance->data;

    switch (parameter_id) {
    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_OPAQUE:
        *val = &_instance->opaque;
        *val_len = sizeof(_instance->opaque);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_STRING:
        *val = &_instance->string;
        *val_len = sizeof(_instance->string);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_BOOLEAN:
        *val = &_instance->boolean;
        *val_len = sizeof(_instance->boolean);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U8:
        *val = &_instance->u8;
        *val_len = sizeof(_instance->u8);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U16:
        *val = &_instance->u16;
        *val_len = sizeof(_instance->u16);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U32:
        *val = &_instance->u32;
        *val_len = sizeof(_instance->u32);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U64:
        *val = &_instance->u64;
        *val_len = sizeof(_instance->u64);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I8:
        *val = &_instance->i8;
        *val_len = sizeof(_instance->i8);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I16:
        *val = &_instance->i16;
        *val_len = sizeof(_instance->i16);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I32:
        *val = &_instance->i32;
        *val_len = sizeof(_instance->i32);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I64:
        *val = &_instance->i64;
        *val_len = sizeof(_instance->i64);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_F32:
        *val = &_instance->f32;
        *val_len = sizeof(_instance->f32);
        break;

    case REGISTRY_TESTS_CONSTRAINED_MIN_MAX_F64:
        *val = &_instance->f64;
        *val_len = sizeof(_instance->f64);
        break;
    }
}

/* Schema */
const registry_parameter_t registry_tests_constrained_min_max_opaque = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_OPAQUE,
    .name = "opaque",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_OPAQUE,
    .constraints.opaque = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_string = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_STRING,
    .name = "string",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_STRING,
    .constraints.string = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_boolean = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_BOOLEAN,
    .name = "boolean",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_BOOL,
    .constraints.boolean = NULL,
};

const registry_parameter_t registry_tests_constrained_min_max_u8 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U8,
    .name = "u8",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (uint8_t[]){ 7 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (uint8_t[]){ 18 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_u16 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U16,
    .name = "u16",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_UINT16,
    .constraints.uint16 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (uint16_t[]){ 7 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (uint16_t[]){ 18 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_u32 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U32,
    .name = "u32",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_UINT32,
    .constraints.uint32 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (uint32_t[]){ 7 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (uint32_t[]){ 18 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_u64 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U64,
    .name = "u64",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_UINT64,
    .constraints.uint64 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (uint64_t[]){ 7 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (uint64_t[]){ 18 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_i8 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I8,
    .name = "i8",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_INT8,
    .constraints.int8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (int8_t[]){ 7 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (int8_t[]){ 18 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_i16 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I16,
    .name = "i16",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_INT16,
    .constraints.int16 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (int16_t[]){ 7 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (int16_t[]){ 18 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_i32 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I32,
    .name = "i32",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_INT32,
    .constraints.int32 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (int32_t[]){ 7 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (int32_t[]){ 18 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_i64 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I64,
    .name = "i64",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_INT64,
    .constraints.int64 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (int64_t[]){ 7 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (int64_t[]){ 18 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_f32 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_F32,
    .name = "f32",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_FLOAT32,
    .constraints.float32 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (float[]){ 7.0 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (float[]){ 18.0 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_constrained_min_max_f64 = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_F64,
    .name = "f64",
    .description = "",
    .schema = &registry_tests_constrained_min_max,
    .type = REGISTRY_TYPE_FLOAT64,
    .constraints.float64 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = (double[]){ 7.0 },
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = (double[]){ 18.0 },
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

registry_schema_t registry_tests_constrained_min_max = {
    .id = REGISTRY_TESTS_CONSTRAINED_MIN_MAX_VALUES,
    .name = "constrained_min_max",
    .description = "",
    .namespace = &registry_tests,
    .mapping = mapping,
    .groups = NULL,
    .groups_len = 0,
    .parameters = (const registry_parameter_t *[]) {
        &registry_tests_constrained_min_max_opaque,
        &registry_tests_constrained_min_max_string,
        &registry_tests_constrained_min_max_boolean,
        &registry_tests_constrained_min_max_u8,
        &registry_tests_constrained_min_max_u16,
        &registry_tests_constrained_min_max_u32,
        &registry_tests_constrained_min_max_u64,
        &registry_tests_constrained_min_max_i8,
        &registry_tests_constrained_min_max_i16,
        &registry_tests_constrained_min_max_i32,
        &registry_tests_constrained_min_max_i64,
        &registry_tests_constrained_min_max_f32,
        &registry_tests_constrained_min_max_f64,
    },
    .parameters_len = 13,
};

#endif
