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
 */

#ifndef REGISTRY_NAMESPACE_TESTS_CONSTRAINED_MIN_MAX_H
#define REGISTRY_NAMESPACE_TESTS_CONSTRAINED_MIN_MAX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"

/* CONSTRAINED_MIN_MAX */
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_CONSTRAINED_MIN_MAX) || IS_ACTIVE(DOXYGEN)

extern const registry_parameter_t registry_tests_constrained_min_max_opaque;
extern const registry_parameter_t registry_tests_constrained_min_max_string;
extern const registry_parameter_t registry_tests_constrained_min_max_boolean;
extern const registry_parameter_t registry_tests_constrained_min_max_u8;
extern const registry_parameter_t registry_tests_constrained_min_max_u16;
extern const registry_parameter_t registry_tests_constrained_min_max_u32;
extern const registry_parameter_t registry_tests_constrained_min_max_u64;
extern const registry_parameter_t registry_tests_constrained_min_max_i8;
extern const registry_parameter_t registry_tests_constrained_min_max_i16;
extern const registry_parameter_t registry_tests_constrained_min_max_i32;
extern const registry_parameter_t registry_tests_constrained_min_max_i64;
extern const registry_parameter_t registry_tests_constrained_min_max_f32;
extern const registry_parameter_t registry_tests_constrained_min_max_f64;
extern registry_schema_t registry_tests_constrained_min_max;

typedef struct {
    uint8_t value;
} registry_tests_constrained_min_max_instance_opaque_t;

typedef struct {
    clist_node_t node;

    registry_tests_constrained_min_max_instance_opaque_t opaque;
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
} registry_tests_constrained_min_max_instance_t;

typedef enum {
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_OPAQUE,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_STRING,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_BOOLEAN,

    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U8,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U16,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U32,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_U64,

    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I8,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I16,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I32,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_I64,

    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_F32,
    REGISTRY_TESTS_CONSTRAINED_MIN_MAX_F64,
} registry_tests_constrained_min_max_indices_t;

#endif

#ifdef __cplusplus
}
#endif

#endif /* CONSTRAINED_MIN_MAX */
/** @} */
