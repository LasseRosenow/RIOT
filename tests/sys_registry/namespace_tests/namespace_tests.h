/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_tests RIOT Registry Tests Namespace
 * @ingroup     tests
 * @brief       RIOT Registry Namespace Tests module providing common tests configuration schemas for the RIOT Registry sys module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_NAMESPACE_TESTS_H
#define REGISTRY_NAMESPACE_TESTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"

/* Tests namespace */

extern registry_namespace_t registry_tests;

/* RGB-LED */
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
    REGISTRY_SCHEMA_FULL_EXAMPLE_STRING,
    REGISTRY_SCHEMA_FULL_EXAMPLE_BOOLEAN,

    REGISTRY_SCHEMA_FULL_EXAMPLE_U8,
    REGISTRY_SCHEMA_FULL_EXAMPLE_U16,
    REGISTRY_SCHEMA_FULL_EXAMPLE_U32,
    REGISTRY_SCHEMA_FULL_EXAMPLE_U64,

    REGISTRY_SCHEMA_FULL_EXAMPLE_I8,
    REGISTRY_SCHEMA_FULL_EXAMPLE_I16,
    REGISTRY_SCHEMA_FULL_EXAMPLE_I32,
    REGISTRY_SCHEMA_FULL_EXAMPLE_I64,

    REGISTRY_SCHEMA_FULL_EXAMPLE_F32,
    REGISTRY_SCHEMA_FULL_EXAMPLE_F64,

} registry_tests_full_indices_t;

#endif

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_NAMESPACE_TESTS_H */
/** @} */
