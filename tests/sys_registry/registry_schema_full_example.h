/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    unittests
 * @brief       Unittests for the ``registry`` module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */
#ifndef TESTS_REGISTRY_SCHEMA_FULL_EXAMPLE_H
#define TESTS_REGISTRY_SCHEMA_FULL_EXAMPLE_H

#include "embUnit.h"
#include "registry.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REGISTRY_SCHEMA_FULL_EXAMPLE 0

extern registry_schema_t registry_schema_full_example;

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
} registry_schema_full_example_t;

typedef enum {
    REGISTRY_SCHEMA_FULL_EXAMPLE_STRING,
    REGISTRY_SCHEMA_FULL_EXAMPLE_BOOL,

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

} registry_schema_full_example_indices_t;

#ifdef __cplusplus
}
#endif

#endif /* TESTS_REGISTRY_SCHEMA_FULL_EXAMPLE_H */
/** @} */
