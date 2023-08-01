/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_tests_full RIOT Registry Schema: Full
 * @ingroup     sys
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
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/full.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_FULL) || IS_ACTIVE(DOXYGEN)

/* Mapping */
static void mapping(const registry_parameter_id_t parameter_id, const registry_instance_t *instance,
                    void **val, size_t *val_len)
{
    registry_tests_full_instance_t *_instance =
        (registry_tests_full_instance_t *)instance->data;

    switch (parameter_id) {
    case REGISTRY_TESTS_FULL_STRING:
        *val = &_instance->string;
        *val_len = sizeof(_instance->string);
        break;

    case REGISTRY_TESTS_FULL_BOOLEAN:
        *val = &_instance->boolean;
        *val_len = sizeof(_instance->boolean);
        break;

    case REGISTRY_TESTS_FULL_U8:
        *val = &_instance->u8;
        *val_len = sizeof(_instance->u8);
        break;

    case REGISTRY_TESTS_FULL_U16:
        *val = &_instance->u16;
        *val_len = sizeof(_instance->u16);
        break;

    case REGISTRY_TESTS_FULL_U32:
        *val = &_instance->u32;
        *val_len = sizeof(_instance->u32);
        break;

    case REGISTRY_TESTS_FULL_U64:
        *val = &_instance->u64;
        *val_len = sizeof(_instance->u64);
        break;

    case REGISTRY_TESTS_FULL_I8:
        *val = &_instance->i8;
        *val_len = sizeof(_instance->i8);
        break;

    case REGISTRY_TESTS_FULL_I16:
        *val = &_instance->i16;
        *val_len = sizeof(_instance->i16);
        break;

    case REGISTRY_TESTS_FULL_I32:
        *val = &_instance->i32;
        *val_len = sizeof(_instance->i32);
        break;

    case REGISTRY_TESTS_FULL_I64:
        *val = &_instance->i64;
        *val_len = sizeof(_instance->i64);
        break;

    case REGISTRY_TESTS_FULL_F32:
        *val = &_instance->f32;
        *val_len = sizeof(_instance->f32);
        break;

    case REGISTRY_TESTS_FULL_F64:
        *val = &_instance->f64;
        *val_len = sizeof(_instance->f64);
        break;
    }
}

/* Schema */
const registry_parameter_t registry_tests_full_opaque = {
    .id = 0,
    .name = "opaque",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_OPAQUE,
    .constraints.opaque = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
    },
};

const registry_parameter_t registry_tests_full_string = {
    .id = 0,
    .name = "string",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_STRING,
    .constraints.string = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
    },
};

const registry_parameter_t registry_tests_full_boolean = {
    .id = 1,
    .name = "boolean",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_BOOL,
    .constraints.boolean = NULL,
};

const registry_parameter_t registry_tests_full_u8 = {
    .id = 2,
    .name = "u8",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_u16 = {
    .id = 3,
    .name = "u16",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_UINT16,
    .constraints.uint16 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_u32 = {
    .id = 4,
    .name = "u32",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_UINT32,
    .constraints.uint32 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_u64 = {
    .id = 5,
    .name = "u64",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_UINT64,
    .constraints.uint64 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_i8 = {
    .id = 2,
    .name = "i8",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_INT8,
    .constraints.int8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_i16 = {
    .id = 3,
    .name = "i16",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_INT16,
    .constraints.int16 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_i32 = {
    .id = 4,
    .name = "i32",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_INT32,
    .constraints.int32 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_i64 = {
    .id = 5,
    .name = "i64",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_INT64,
    .constraints.int64 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_f32 = {
    .id = 4,
    .name = "f32",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_FLOAT32,
    .constraints.float32 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_tests_full_f64 = {
    .id = 5,
    .name = "f64",
    .description = "",
    .schema = &registry_tests_full,
    .type = REGISTRY_TYPE_FLOAT64,
    .constraints.int64 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

registry_schema_t registry_tests_full = {
    .id = 0,
    .name = "full",
    .description = "",
    .namespace = &registry_tests,
    .mapping = mapping,
    .groups = NULL,
    .groups_len = 0,
    .parameters = (const registry_parameter_t *[]) {
        &registry_tests_full_opaque,
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
    .parameters_len = 13,
};

#endif
