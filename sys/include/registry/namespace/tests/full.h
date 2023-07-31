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
 */

#ifndef REGISTRY_NAMESPACE_TESTS_FULL_H
#define REGISTRY_NAMESPACE_TESTS_FULL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"

/* FULL */
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_FULL) || IS_ACTIVE(DOXYGEN)

extern const registry_resource_t registry_tests_full_string;
extern const registry_resource_t registry_tests_full_boolean;
extern const registry_resource_t registry_tests_full_u8;
extern const registry_resource_t registry_tests_full_u16;
extern const registry_resource_t registry_tests_full_u32;
extern const registry_resource_t registry_tests_full_u64;
extern const registry_resource_t registry_tests_full_i8;
extern const registry_resource_t registry_tests_full_i16;
extern const registry_resource_t registry_tests_full_i32;
extern const registry_resource_t registry_tests_full_i64;
extern const registry_resource_t registry_tests_full_f32;
extern const registry_resource_t registry_tests_full_f64;
extern registry_schema_t registry_tests_full;

typedef struct {
    clist_node_t node;

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
} registry_tests_full_instance_t;

typedef enum {
    REGISTRY_TESTS_FULL_STRING,
    REGISTRY_TESTS_FULL_BOOLEAN,

    REGISTRY_TESTS_FULL_U8,
    REGISTRY_TESTS_FULL_U16,
    REGISTRY_TESTS_FULL_U32,
    REGISTRY_TESTS_FULL_U64,

    REGISTRY_TESTS_FULL_I8,
    REGISTRY_TESTS_FULL_I16,
    REGISTRY_TESTS_FULL_I32,
    REGISTRY_TESTS_FULL_I64,

    REGISTRY_TESTS_FULL_F32,
    REGISTRY_TESTS_FULL_F64,

} registry_tests_full_indices_t;

#endif

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_NAMESPACE_SYS_RGB_LED_H */
/** @} */
