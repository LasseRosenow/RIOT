/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_tests_constrained_forbidden_values RIOT Registry Schema: Constrained Forbidden Values
 * @ingroup     sys
 * @brief       RIOT Registry Constrained Forbidden Values Schema representing all possible data types of the riot registry with forbidden values constraints
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_NAMESPACE_TESTS_CONSTRAINED_FORBIDDEN_VALUES_H
#define REGISTRY_NAMESPACE_TESTS_CONSTRAINED_FORBIDDEN_VALUES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"

/* CONSTRAINED_FORBIDDEN_VALUES */
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_CONSTRAINED_FORBIDDEN_VALUES) || IS_ACTIVE(DOXYGEN)

extern const registry_parameter_t registry_tests_constrained_forbidden_values_opaque;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_string;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_boolean;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_u8;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_u16;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_u32;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_u64;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_i8;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_i16;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_i32;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_i64;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_f32;
extern const registry_parameter_t registry_tests_constrained_forbidden_values_f64;
extern registry_schema_t registry_tests_constrained_forbidden_values;

typedef struct {
    uint8_t value;
} registry_tests_constrained_forbidden_values_instance_opaque_t;

typedef struct {
    clist_node_t node;

    registry_tests_constrained_forbidden_values_instance_opaque_t opaque;
    char string[50];
    bool boolean;

    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;

    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;

    float f32;
    double f64;
} registry_tests_constrained_forbidden_values_instance_t;

typedef enum {
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_OPAQUE,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_STRING,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_BOOLEAN,

    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_U8,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_U16,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_U32,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_U64,

    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_I8,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_I16,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_I32,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_I64,

    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_F32,
    REGISTRY_TESTS_CONSTRAINED_FORBIDDEN_VALUES_F64,
} registry_tests_constrained_forbidden_values_indices_t;

#endif

#ifdef __cplusplus
}
#endif

#endif /* CONSTRAINED_FORBIDDEN_VALUES */
/** @} */
