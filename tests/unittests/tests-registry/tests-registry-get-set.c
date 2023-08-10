/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 * @brief       Unittests for registry_get, registry_set
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <inttypes.h>
#include <errno.h>
#include "embUnit.h"
#include "fmt.h"
#include "assert.h"
#include "vfs.h"
#include "board.h"
#include "mtd.h"
#include "registry.h"

#include "tests-registry.h"
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/full.h"
#include "registry/namespace/tests/constrained_min_max.h"
#include "registry/namespace/tests/constrained_allowed_values.h"
#include "registry/namespace/tests/constrained_forbidden_values.h"

#define FLOAT_MAX_CHAR_COUNT ((FLT_MAX_10_EXP + 1) + 1 + 1 + 6)     // (FLT_MAX_10_EXP + 1) + sign + dot + 6 decimal places
#define DOUBLE_MAX_CHAR_COUNT ((DBL_MAX_10_EXP + 1) + 1 + 1 + 6)    // (DBL_MAX_10_EXP + 1) + sign + dot + 6 decimal places

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_FULL) || IS_ACTIVE(DOXYGEN)

static int commit_cb(const registry_commit_cb_scope_t scope,
                     const registry_group_or_parameter_id_t *group_or_parameter_id,
                     const void *context)
{
    (void)scope;
    (void)group_or_parameter_id;
    (void)context;

    return 0;
}

static registry_tests_full_instance_t test_full_instance_1_data = {
    .opaque = {
        .value = 7,
    },
    .string = "hello world",
    .boolean = true,
    .u8 = 9,
    .u16 = 17,
    .u32 = 33,
    .u64 = 65,
    .i8 = 8,
    .i16 = 16,
    .i32 = 32,
    .i64 = 64,
    .f32 = 3.2,
    .f64 = 6.4,
};

static registry_instance_t test_full_instance_1 = {
    .name = "test-full-1",
    .data = &test_full_instance_1_data,
    .commit_cb = &commit_cb,
};

static registry_tests_constrained_min_max_instance_t test_constrained_min_max_instance_1_data = {
    .opaque = {
        .value = 7,
    },
    .string = "hello world",
    .boolean = true,
    .u8 = 9,
    .u16 = 17,
    .u32 = 33,
    .u64 = 65,
    .i8 = 8,
    .i16 = 16,
    .i32 = 32,
    .i64 = 64,
    .f32 = 3.2,
    .f64 = 6.4,
};

static registry_instance_t test_constrained_min_max_instance_1 = {
    .name = "test-constrained_min_max-1",
    .data = &test_constrained_min_max_instance_1_data,
    .commit_cb = &commit_cb,
};

static registry_tests_constrained_allowed_values_instance_t
    test_constrained_allowed_values_instance_1_data = {
    .opaque = {
        .value = 7,
    },
    .string = "hello world",
    .boolean = true,
    .u8 = 9,
    .u16 = 17,
    .u32 = 33,
    .u64 = 65,
    .i8 = 8,
    .i16 = 16,
    .i32 = 32,
    .i64 = 64,
    .f32 = 3.2,
    .f64 = 6.4,
};

static registry_instance_t test_constrained_allowed_values_instance_1 = {
    .name = "test-constrained_allowed_values-1",
    .data = &test_constrained_allowed_values_instance_1_data,
    .commit_cb = &commit_cb,
};

static registry_tests_constrained_forbidden_values_instance_t
    test_constrained_forbidden_values_instance_1_data = {
    .opaque = {
        .value = 7,
    },
    .string = "hello world",
    .boolean = true,
    .u8 = 9,
    .u16 = 17,
    .u32 = 33,
    .u64 = 65,
    .i8 = 8,
    .i16 = 16,
    .i32 = 32,
    .i64 = 64,
    .f32 = 3.2,
    .f64 = 6.4,
};

static registry_instance_t test_constrained_forbidden_values_instance_1 = {
    .name = "test-constrained_forbidden_values-1",
    .data = &test_constrained_forbidden_values_instance_1_data,
    .commit_cb = &commit_cb,
};

static void test_registry_setup(void)
{
    /* init registry */
    registry_init();

    /* add schema instances */
    registry_register_schema_instance(&registry_tests_full, &test_full_instance_1);
    registry_register_schema_instance(&registry_tests_constrained_min_max,
                                      &test_constrained_min_max_instance_1);
}

static void test_registry_teardown(void)
{
}

static void tests_registry_min_values(void)
{
    registry_value_t output;

    /* opaque */
    const registry_tests_full_instance_opaque_t input_opaque = {
        .value = 0,
    };

    registry_set(&test_full_instance_1, &registry_tests_full_opaque, &input_opaque,
                 sizeof(input_opaque));
    registry_get(&test_full_instance_1, &registry_tests_full_opaque, &output);

    TEST_ASSERT_EQUAL_INT(input_opaque.value,
                          ((registry_tests_full_instance_opaque_t *)output.buf)->value);

    /* string */
    const char input_string[] = "";

    registry_set(&test_full_instance_1, &registry_tests_full_string, input_string,
                 sizeof(input_string));
    registry_get(&test_full_instance_1, &registry_tests_full_string, &output);

    TEST_ASSERT_EQUAL_STRING("", (char *)output.buf);


    /* bool */
    const bool input_bool = false;

    registry_set(&test_full_instance_1, &registry_tests_full_boolean, &input_bool,
                 sizeof(input_bool));
    registry_get(&test_full_instance_1, &registry_tests_full_boolean, &output);

    TEST_ASSERT_EQUAL_INT(input_bool, *(bool *)output.buf);


    /* u8 */
    const uint8_t input_u8 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_u8, &input_u8, sizeof(input_u8));
    registry_get(&test_full_instance_1, &registry_tests_full_u8, &output);

    TEST_ASSERT_EQUAL_INT(input_u8, *(uint8_t *)output.buf);


    /* u16 */
    const uint16_t input_u16 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_u16, &input_u16, sizeof(input_u16));
    registry_get(&test_full_instance_1, &registry_tests_full_u16, &output);

    TEST_ASSERT_EQUAL_INT(input_u16, *(uint16_t *)output.buf);


    /* u32 */
    const uint32_t input_u32 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_u32, &input_u32, sizeof(input_u32));
    registry_get(&test_full_instance_1, &registry_tests_full_u32, &output);

    TEST_ASSERT_EQUAL_INT(input_u32, *(uint32_t *)output.buf);


    /* u64 */
    const uint64_t input_u64 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_u64, &input_u64, sizeof(input_u64));
    registry_get(&test_full_instance_1, &registry_tests_full_u64, &output);

    TEST_ASSERT_EQUAL_INT(input_u64, *(uint64_t *)output.buf);


    /* i8 */
    const int8_t input_i8 = INT8_MIN;

    registry_set(&test_full_instance_1, &registry_tests_full_i8, &input_i8, sizeof(input_i8));
    registry_get(&test_full_instance_1, &registry_tests_full_i8, &output);

    TEST_ASSERT_EQUAL_INT(input_i8, *(int8_t *)output.buf);


    /* i16 */
    const int16_t input_i16 = INT16_MIN;

    registry_set(&test_full_instance_1, &registry_tests_full_i16, &input_i16, sizeof(input_i16));
    registry_get(&test_full_instance_1, &registry_tests_full_i16, &output);

    TEST_ASSERT_EQUAL_INT(input_i16, *(int16_t *)output.buf);


    /* i32 */
    const int32_t input_i32 = INT32_MIN;

    registry_set(&test_full_instance_1, &registry_tests_full_i32, &input_i32, sizeof(input_i32));
    registry_get(&test_full_instance_1, &registry_tests_full_i32, &output);

    TEST_ASSERT_EQUAL_INT(input_i32, *(int32_t *)output.buf);


    /* i64 */
    const int64_t input_i64 = INT64_MIN;

    registry_set(&test_full_instance_1, &registry_tests_full_i64, &input_i64, sizeof(input_i64));
    registry_get(&test_full_instance_1, &registry_tests_full_i64, &output);

    TEST_ASSERT_EQUAL_INT(input_i64, *(int64_t *)output.buf);


    /* f32 */
    const float input_f32 = FLT_MIN;

    registry_set(&test_full_instance_1, &registry_tests_full_f32, &input_f32, sizeof(input_f32));
    registry_get(&test_full_instance_1, &registry_tests_full_f32, &output);

    char input_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f32_string, "%f", input_f32);
    sprintf(output_f32_string, "%f", *(float *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f32_string, output_f32_string);


    /* f64 */
    const double input_f64 = DBL_MIN;

    registry_set(&test_full_instance_1, &registry_tests_full_f64, &input_f64, sizeof(input_f64));
    registry_get(&test_full_instance_1, &registry_tests_full_f64, &output);

    char input_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f64_string, "%f", input_f64);
    sprintf(output_f64_string, "%f", *(double *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f64_string, output_f64_string);
}

static void tests_registry_zero_values(void)
{
    registry_value_t output;

    /* opaque */
    const registry_tests_full_instance_opaque_t input_opaque = {
        .value = 0,
    };

    registry_set(&test_full_instance_1, &registry_tests_full_opaque, &input_opaque,
                 sizeof(input_opaque));
    registry_get(&test_full_instance_1, &registry_tests_full_opaque, &output);

    TEST_ASSERT_EQUAL_INT(input_opaque.value,
                          ((registry_tests_full_instance_opaque_t *)output.buf)->value);


    /* string */
    const char input_string[] = "";

    registry_set(&test_full_instance_1, &registry_tests_full_string, input_string,
                 sizeof(input_string));
    registry_get(&test_full_instance_1, &registry_tests_full_string, &output);

    TEST_ASSERT_EQUAL_STRING("", (char *)output.buf);


    /* bool */
    const bool input_bool = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_boolean, &input_bool,
                 sizeof(input_bool));
    registry_get(&test_full_instance_1, &registry_tests_full_boolean, &output);

    TEST_ASSERT_EQUAL_INT(input_bool, *(bool *)output.buf);


    /* u8 */
    const uint8_t input_u8 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_u8, &input_u8, sizeof(input_u8));
    registry_get(&test_full_instance_1, &registry_tests_full_u8, &output);

    TEST_ASSERT_EQUAL_INT(input_u8, *(uint8_t *)output.buf);


    /* u16 */
    const uint16_t input_u16 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_u16, &input_u16, sizeof(input_u16));
    registry_get(&test_full_instance_1, &registry_tests_full_u16, &output);

    TEST_ASSERT_EQUAL_INT(input_u16, *(uint16_t *)output.buf);


    /* u32 */
    const uint32_t input_u32 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_u32, &input_u32, sizeof(input_u32));
    registry_get(&test_full_instance_1, &registry_tests_full_u32, &output);

    TEST_ASSERT_EQUAL_INT(input_u32, *(uint32_t *)output.buf);


    /* u64 */
    const uint64_t input_u64 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_u64, &input_u64, sizeof(input_u64));
    registry_get(&test_full_instance_1, &registry_tests_full_u64, &output);

    TEST_ASSERT_EQUAL_INT(input_u64, *(uint64_t *)output.buf);


    /* i8 */
    const int8_t input_i8 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_i8, &input_i8, sizeof(input_i8));
    registry_get(&test_full_instance_1, &registry_tests_full_i8, &output);

    TEST_ASSERT_EQUAL_INT(input_i8, *(int8_t *)output.buf);


    /* i16 */
    const int16_t input_i16 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_i16, &input_i16, sizeof(input_i16));
    registry_get(&test_full_instance_1, &registry_tests_full_i16, &output);

    TEST_ASSERT_EQUAL_INT(input_i16, *(int16_t *)output.buf);


    /* i32 */
    const int32_t input_i32 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_i32, &input_i32, sizeof(input_i32));
    registry_get(&test_full_instance_1, &registry_tests_full_i32, &output);

    TEST_ASSERT_EQUAL_INT(input_i32, *(int32_t *)output.buf);


    /* i64 */
    const int64_t input_i64 = 0;

    registry_set(&test_full_instance_1, &registry_tests_full_i64, &input_i64, sizeof(input_i64));
    registry_get(&test_full_instance_1, &registry_tests_full_i64, &output);

    TEST_ASSERT_EQUAL_INT(input_i64, *(int64_t *)output.buf);


    /* f32 */
    const float input_f32 = 0.0;

    registry_set(&test_full_instance_1, &registry_tests_full_f32, &input_f32, sizeof(input_f32));
    registry_get(&test_full_instance_1, &registry_tests_full_f32, &output);

    char input_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f32_string, "%f", input_f32);
    sprintf(output_f32_string, "%f", *(float *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f32_string, output_f32_string);


    /* f64 */
    const double input_f64 = 0.0;

    registry_set(&test_full_instance_1, &registry_tests_full_f64, &input_f64, sizeof(input_f64));
    registry_get(&test_full_instance_1, &registry_tests_full_f64, &output);

    char input_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f64_string, "%f", input_f64);
    sprintf(output_f64_string, "%f", *(double *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f64_string, output_f64_string);
}

static void tests_registry_max_values(void)
{
    registry_value_t output;

    /* opaque */
    const registry_tests_full_instance_opaque_t input_opaque = {
        .value = UINT8_MAX,
    };

    registry_set(&test_full_instance_1, &registry_tests_full_opaque, &input_opaque,
                 sizeof(input_opaque));
    registry_get(&test_full_instance_1, &registry_tests_full_opaque, &output);

    TEST_ASSERT_EQUAL_INT(input_opaque.value,
                          ((registry_tests_full_instance_opaque_t *)output.buf)->value);


    /* string */
    char input_string[sizeof(test_full_instance_1_data.string)] = { 0 };

    for (size_t i = 0; i < sizeof(input_string) - 1; i++) {
        input_string[i] = 'a';
    }

    registry_set(&test_full_instance_1, &registry_tests_full_string, input_string,
                 sizeof(input_string));
    registry_get(&test_full_instance_1, &registry_tests_full_string, &output);

    TEST_ASSERT_EQUAL_STRING(input_string, (char *)output.buf);


    /* bool */
    const bool input_bool = true;

    registry_set(&test_full_instance_1, &registry_tests_full_boolean, &input_bool,
                 sizeof(input_bool));
    registry_get(&test_full_instance_1, &registry_tests_full_boolean, &output);

    TEST_ASSERT_EQUAL_INT(input_bool, *(bool *)output.buf);


    /* u8 */
    const uint8_t input_u8 = UINT8_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_u8, &input_u8, sizeof(input_u8));
    registry_get(&test_full_instance_1, &registry_tests_full_u8, &output);

    TEST_ASSERT_EQUAL_INT(input_u8, *(uint8_t *)output.buf);


    /* u16 */
    const uint16_t input_u16 = UINT16_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_u16, &input_u16, sizeof(input_u16));
    registry_get(&test_full_instance_1, &registry_tests_full_u16, &output);

    TEST_ASSERT_EQUAL_INT(input_u16, *(uint16_t *)output.buf);


    /* u32 */
    const uint32_t input_u32 = UINT32_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_u32, &input_u32, sizeof(input_u32));
    registry_get(&test_full_instance_1, &registry_tests_full_u32, &output);

    TEST_ASSERT_EQUAL_INT(input_u32, *(uint32_t *)output.buf);


    /* u64 */
    const uint64_t input_u64 = UINT64_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_u64, &input_u64, sizeof(input_u64));
    registry_get(&test_full_instance_1, &registry_tests_full_u64, &output);

    TEST_ASSERT_EQUAL_INT(input_u64, *(uint64_t *)output.buf);


    /* i8 */
    const int8_t input_i8 = INT8_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_i8, &input_i8, sizeof(input_i8));
    registry_get(&test_full_instance_1, &registry_tests_full_i8, &output);

    TEST_ASSERT_EQUAL_INT(input_i8, *(int8_t *)output.buf);


    /* i16 */
    const int16_t input_i16 = INT16_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_i16, &input_i16, sizeof(input_i16));
    registry_get(&test_full_instance_1, &registry_tests_full_i16, &output);

    TEST_ASSERT_EQUAL_INT(input_i16, *(int16_t *)output.buf);


    /* i32 */
    const int32_t input_i32 = INT32_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_i32, &input_i32, sizeof(input_i32));
    registry_get(&test_full_instance_1, &registry_tests_full_i32, &output);

    TEST_ASSERT_EQUAL_INT(input_i32, *(int32_t *)output.buf);


    /* i64 */
    const int64_t input_i64 = INT64_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_i64, &input_i64, sizeof(input_i64));
    registry_get(&test_full_instance_1, &registry_tests_full_i64, &output);

    TEST_ASSERT_EQUAL_INT(input_i64, *(int64_t *)output.buf);


    /* f32 */
    const float input_f32 = FLT_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_f32, &input_f32, sizeof(input_f32));
    registry_get(&test_full_instance_1, &registry_tests_full_f32, &output);

    char input_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f32_string, "%f", input_f32);
    sprintf(output_f32_string, "%f", *(float *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f32_string, output_f32_string);


    /* f64 */
    const double input_f64 = DBL_MAX;

    registry_set(&test_full_instance_1, &registry_tests_full_f64, &input_f64, sizeof(input_f64));
    registry_get(&test_full_instance_1, &registry_tests_full_f64, &output);

    char input_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f64_string, "%f", input_f64);
    sprintf(output_f64_string, "%f", *(double *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f64_string, output_f64_string);
}

static void tests_registry_constraints_min_max(void)
{
    int res_too_small;
    int res_too_large;
    int res_within_range;

    /* opaque */
    /* opaque does not have min_max constraints */

    /* string */
    /* string does not have min_max constraints */

    /* bool */
    /* bool does not have min_max constraints */

    /* u8 */
    const uint8_t input_u8_too_small =
        *registry_tests_constrained_min_max_u8.constraints.uint8.min_value -
        1;
    const uint8_t input_u8_too_large =
        *registry_tests_constrained_min_max_u8.constraints.uint8.max_value +
        1;
    const uint8_t input_u8_within_range =
        *registry_tests_constrained_min_max_u8.constraints.uint8.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_u8,
                                 &input_u8_too_small, sizeof(input_u8_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_u8,
                                 &input_u8_too_large, sizeof(input_u8_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_u8,
                                    &input_u8_within_range, sizeof(input_u8_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* u16 */
    const uint16_t input_u16_too_small =
        *registry_tests_constrained_min_max_u16.constraints.uint16.min_value -
        1;
    const uint16_t input_u16_too_large =
        *registry_tests_constrained_min_max_u16.constraints.uint16.max_value +
        1;
    const uint16_t input_u16_within_range =
        *registry_tests_constrained_min_max_u16.constraints.uint16.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_u16,
                                 &input_u16_too_small, sizeof(input_u16_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_u16,
                                 &input_u16_too_large, sizeof(input_u16_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_u16,
                                    &input_u16_within_range, sizeof(input_u16_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* u32 */
    const uint32_t input_u32_too_small =
        *registry_tests_constrained_min_max_u32.constraints.uint32.min_value -
        1;
    const uint32_t input_u32_too_large =
        *registry_tests_constrained_min_max_u32.constraints.uint32.max_value +
        1;
    const uint32_t input_u32_within_range =
        *registry_tests_constrained_min_max_u32.constraints.uint32.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_u32,
                                 &input_u32_too_small, sizeof(input_u32_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_u32,
                                 &input_u32_too_large, sizeof(input_u32_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_u32,
                                    &input_u32_within_range, sizeof(input_u32_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* u64 */
    const uint64_t input_u64_too_small =
        *registry_tests_constrained_min_max_u64.constraints.uint64.min_value -
        1;
    const uint64_t input_u64_too_large =
        *registry_tests_constrained_min_max_u64.constraints.uint64.max_value +
        1;
    const uint64_t input_u64_within_range =
        *registry_tests_constrained_min_max_u64.constraints.uint64.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_u64,
                                 &input_u64_too_small, sizeof(input_u64_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_u64,
                                 &input_u64_too_large, sizeof(input_u64_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_u64,
                                    &input_u64_within_range, sizeof(input_u64_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* i8 */
    const uint8_t input_i8_too_small =
        *registry_tests_constrained_min_max_i8.constraints.int8.min_value -
        1;
    const int8_t input_i8_too_large =
        *registry_tests_constrained_min_max_i8.constraints.int8.max_value +
        1;
    const int8_t input_i8_within_range =
        *registry_tests_constrained_min_max_i8.constraints.int8.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_i8,
                                 &input_i8_too_small, sizeof(input_i8_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_i8,
                                 &input_i8_too_large, sizeof(input_i8_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_i8,
                                    &input_i8_within_range, sizeof(input_i8_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* i16 */
    const int16_t input_i16_too_small =
        *registry_tests_constrained_min_max_i16.constraints.int16.min_value -
        1;
    const int16_t input_i16_too_large =
        *registry_tests_constrained_min_max_i16.constraints.int16.max_value +
        1;
    const int16_t input_i16_within_range =
        *registry_tests_constrained_min_max_i16.constraints.int16.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_i16,
                                 &input_i16_too_small, sizeof(input_i16_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_i16,
                                 &input_i16_too_large, sizeof(input_i16_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_i16,
                                    &input_i16_within_range, sizeof(input_i16_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* i32 */
    const int32_t input_i32_too_small =
        *registry_tests_constrained_min_max_i32.constraints.int32.min_value -
        1;
    const int32_t input_i32_too_large =
        *registry_tests_constrained_min_max_i32.constraints.int32.max_value +
        1;
    const int32_t input_i32_within_range =
        *registry_tests_constrained_min_max_i32.constraints.int32.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_i32,
                                 &input_i32_too_small, sizeof(input_i32_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_i32,
                                 &input_i32_too_large, sizeof(input_i32_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_i32,
                                    &input_i32_within_range, sizeof(input_i32_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* i64 */
    const int64_t input_i64_too_small =
        *registry_tests_constrained_min_max_i64.constraints.int64.min_value -
        1;
    const int64_t input_i64_too_large =
        *registry_tests_constrained_min_max_i64.constraints.int64.max_value +
        1;
    const int64_t input_i64_within_range =
        *registry_tests_constrained_min_max_i64.constraints.int64.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_i64,
                                 &input_i64_too_small, sizeof(input_i64_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_i64,
                                 &input_i64_too_large, sizeof(input_i64_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_i64,
                                    &input_i64_within_range, sizeof(input_i64_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* f32 */
    const float input_f32_too_small =
        *registry_tests_constrained_min_max_f32.constraints.float32.min_value -
        1.0;
    const float input_f32_too_large =
        *registry_tests_constrained_min_max_f32.constraints.float32.max_value +
        1.0;
    const float input_f32_within_range =
        *registry_tests_constrained_min_max_f32.constraints.float32.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_f32,
                                 &input_f32_too_small, sizeof(input_f32_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_f32,
                                 &input_f32_too_large, sizeof(input_f32_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_f32,
                                    &input_f32_within_range, sizeof(input_f32_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);


    /* f64 */
    const double input_f64_too_small =
        *registry_tests_constrained_min_max_f64.constraints.float64.min_value -
        1.0;
    const double input_f64_too_large =
        *registry_tests_constrained_min_max_f64.constraints.float64.max_value +
        1.0;
    const double input_f64_within_range =
        *registry_tests_constrained_min_max_f64.constraints.float64.min_value;

    res_too_small = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_f64,
                                 &input_f64_too_small, sizeof(input_f64_too_small));

    res_too_large = registry_set(&test_constrained_min_max_instance_1,
                                 &registry_tests_constrained_min_max_f64,
                                 &input_f64_too_large, sizeof(input_f64_too_large));

    res_within_range = registry_set(&test_constrained_min_max_instance_1,
                                    &registry_tests_constrained_min_max_f64,
                                    &input_f64_within_range, sizeof(input_f64_within_range));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_small);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_too_large);
    TEST_ASSERT_EQUAL_INT(0, res_within_range);
}

static void tests_registry_constraints_allowed_values(void)
{
    int res_allowed;
    int res_other;

    /* opaque */
    const registry_tests_constrained_allowed_values_instance_opaque_t *input_opaque_allowed =
        registry_tests_constrained_allowed_values_opaque.constraints.opaque.allowed_values[0];
    const registry_tests_constrained_allowed_values_instance_opaque_t input_opaque_other = {
        .value = 19,
    };

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_opaque,
                               input_opaque_allowed, sizeof(*input_opaque_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_opaque,
                             &input_opaque_other, sizeof(input_opaque_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* string */
    const char *input_string_allowed =
        registry_tests_constrained_allowed_values_string.constraints.string.allowed_values[0];
    const char *input_string_other = "test test";

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_string,
                               input_string_allowed, sizeof(input_string_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_string,
                             input_string_other, sizeof(input_string_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* bool */
    /* bool does not have allowed values constraints */

    /* u8 */
    const uint8_t input_u8_allowed =
        registry_tests_constrained_allowed_values_u8.constraints.uint8.allowed_values[0];
    const uint8_t input_u8_other =
        registry_tests_constrained_allowed_values_u8.constraints.uint8.allowed_values[0] + 1;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_u8,
                               &input_u8_allowed, sizeof(input_u8_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_u8,
                             &input_u8_other, sizeof(input_u8_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* u16 */
    const uint16_t input_u16_allowed =
        registry_tests_constrained_allowed_values_u16.constraints.uint16.allowed_values[0];
    const uint16_t input_u16_other =
        registry_tests_constrained_allowed_values_u16.constraints.uint16.allowed_values[0] + 1;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_u16,
                               &input_u16_allowed, sizeof(input_u16_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_u16,
                             &input_u16_other, sizeof(input_u16_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* u32 */
    const uint32_t input_u32_allowed =
        registry_tests_constrained_allowed_values_u32.constraints.uint32.allowed_values[0];
    const uint32_t input_u32_other =
        registry_tests_constrained_allowed_values_u32.constraints.uint32.allowed_values[0] + 1;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_u32,
                               &input_u32_allowed, sizeof(input_u32_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_u32,
                             &input_u32_other, sizeof(input_u16_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* u64 */
    const uint64_t input_u64_allowed =
        registry_tests_constrained_allowed_values_u64.constraints.uint64.allowed_values[0];
    const uint64_t input_u64_other =
        registry_tests_constrained_allowed_values_u64.constraints.uint64.allowed_values[0] + 1;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_u64,
                               &input_u64_allowed, sizeof(input_u64_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_u64,
                             &input_u64_other, sizeof(input_u16_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* i8 */
    const int8_t input_i8_allowed =
        registry_tests_constrained_allowed_values_i8.constraints.int8.allowed_values[0];
    const int8_t input_i8_other =
        registry_tests_constrained_allowed_values_i8.constraints.int8.allowed_values[0] + 1;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_i8,
                               &input_i8_allowed, sizeof(input_i8_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_i8,
                             &input_i8_other, sizeof(input_i8_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* i16 */
    const int16_t input_i16_allowed =
        registry_tests_constrained_allowed_values_i16.constraints.int16.allowed_values[0];
    const int16_t input_i16_other =
        registry_tests_constrained_allowed_values_i16.constraints.int16.allowed_values[0] + 1;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_i16,
                               &input_i16_allowed, sizeof(input_i16_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_i16,
                             &input_i16_other, sizeof(input_i16_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* i32 */
    const int32_t input_i32_allowed =
        registry_tests_constrained_allowed_values_i32.constraints.int32.allowed_values[0];
    const int32_t input_i32_other =
        registry_tests_constrained_allowed_values_i32.constraints.int32.allowed_values[0] + 1;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_i32,
                               &input_i32_allowed, sizeof(input_i32_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_i32,
                             &input_i32_other, sizeof(input_i32_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);


    /* i64 */
    const int64_t input_i64_allowed =
        registry_tests_constrained_allowed_values_i64.constraints.int64.allowed_values[0];
    const int64_t input_i64_other =
        registry_tests_constrained_allowed_values_i64.constraints.int64.allowed_values[0] + 1;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_i64,
                               &input_i64_allowed, sizeof(input_i64_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_i64,
                             &input_i64_other, sizeof(input_i64_other));


    /* f32 */
    const float input_f32_allowed =
        registry_tests_constrained_allowed_values_f32.constraints.float32.allowed_values[0];
    const float input_f32_other =
        registry_tests_constrained_allowed_values_f32.constraints.float32.allowed_values[0] + 1.0;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_f32,
                               &input_f32_allowed, sizeof(input_f32_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_f32,
                             &input_f32_other, sizeof(input_f32_other));


    /* f64 */
    const double input_f64_allowed =
        registry_tests_constrained_allowed_values_f64.constraints.float64.allowed_values[0];
    const double input_f64_other =
        registry_tests_constrained_allowed_values_f64.constraints.float64.allowed_values[0] + 1.0;

    res_allowed = registry_set(&test_constrained_allowed_values_instance_1,
                               &registry_tests_constrained_allowed_values_f64,
                               &input_f64_allowed, sizeof(input_f64_allowed));

    res_other = registry_set(&test_constrained_allowed_values_instance_1,
                             &registry_tests_constrained_allowed_values_f64,
                             &input_f64_other, sizeof(input_f64_other));

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);

    TEST_ASSERT_EQUAL_INT(0, res_allowed);
    TEST_ASSERT_EQUAL_INT(-EINVAL, res_other);
}

static void tests_registry_constraints_forbidden_values(void)
{
    int res_forbidden;
    int res_other;

    /* opaque */
    const registry_tests_constrained_forbidden_values_instance_opaque_t *input_opaque_forbidden =
        registry_tests_constrained_forbidden_values_opaque.constraints.opaque.forbidden_values[0];
    const registry_tests_constrained_forbidden_values_instance_opaque_t input_opaque_other = {
        .value = 19,
    };

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_opaque,
                                 input_opaque_forbidden, sizeof(*input_opaque_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_opaque,
                             &input_opaque_other, sizeof(input_opaque_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* string */
    const char *input_string_forbidden =
        registry_tests_constrained_forbidden_values_string.constraints.string.forbidden_values[0];
    const char *input_string_other = "test test";

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_string,
                                 input_string_forbidden, sizeof(input_string_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_string,
                             input_string_other, sizeof(input_string_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* bool */
    /* bool does not have forbidden values constraints */

    /* u8 */
    const uint8_t input_u8_forbidden =
        registry_tests_constrained_forbidden_values_u8.constraints.uint8.forbidden_values[0];
    const uint8_t input_u8_other =
        registry_tests_constrained_forbidden_values_u8.constraints.uint8.forbidden_values[0] + 1;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_u8,
                                 &input_u8_forbidden, sizeof(input_u8_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_u8,
                             &input_u8_other, sizeof(input_u8_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* u16 */
    const uint16_t input_u16_forbidden =
        registry_tests_constrained_forbidden_values_u16.constraints.uint16.forbidden_values[0];
    const uint16_t input_u16_other =
        registry_tests_constrained_forbidden_values_u16.constraints.uint16.forbidden_values[0] + 1;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_u16,
                                 &input_u16_forbidden, sizeof(input_u16_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_u16,
                             &input_u16_other, sizeof(input_u16_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* u32 */
    const uint32_t input_u32_forbidden =
        registry_tests_constrained_forbidden_values_u32.constraints.uint32.forbidden_values[0];
    const uint32_t input_u32_other =
        registry_tests_constrained_forbidden_values_u32.constraints.uint32.forbidden_values[0] + 1;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_u32,
                                 &input_u32_forbidden, sizeof(input_u32_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_u32,
                             &input_u32_other, sizeof(input_u16_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* u64 */
    const uint64_t input_u64_forbidden =
        registry_tests_constrained_forbidden_values_u64.constraints.uint64.forbidden_values[0];
    const uint64_t input_u64_other =
        registry_tests_constrained_forbidden_values_u64.constraints.uint64.forbidden_values[0] + 1;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_u64,
                                 &input_u64_forbidden, sizeof(input_u64_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_u64,
                             &input_u64_other, sizeof(input_u16_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* i8 */
    const int8_t input_i8_forbidden =
        registry_tests_constrained_forbidden_values_i8.constraints.int8.forbidden_values[0];
    const int8_t input_i8_other =
        registry_tests_constrained_forbidden_values_i8.constraints.int8.forbidden_values[0] + 1;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_i8,
                                 &input_i8_forbidden, sizeof(input_i8_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_i8,
                             &input_i8_other, sizeof(input_i8_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* i16 */
    const int16_t input_i16_forbidden =
        registry_tests_constrained_forbidden_values_i16.constraints.int16.forbidden_values[0];
    const int16_t input_i16_other =
        registry_tests_constrained_forbidden_values_i16.constraints.int16.forbidden_values[0] + 1;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_i16,
                                 &input_i16_forbidden, sizeof(input_i16_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_i16,
                             &input_i16_other, sizeof(input_i16_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* i32 */
    const int32_t input_i32_forbidden =
        registry_tests_constrained_forbidden_values_i32.constraints.int32.forbidden_values[0];
    const int32_t input_i32_other =
        registry_tests_constrained_forbidden_values_i32.constraints.int32.forbidden_values[0] + 1;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_i32,
                                 &input_i32_forbidden, sizeof(input_i32_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_i32,
                             &input_i32_other, sizeof(input_i32_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* i64 */
    const int64_t input_i64_forbidden =
        registry_tests_constrained_forbidden_values_i64.constraints.int64.forbidden_values[0];
    const int64_t input_i64_other =
        registry_tests_constrained_forbidden_values_i64.constraints.int64.forbidden_values[0] + 1;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_i64,
                                 &input_i64_forbidden, sizeof(input_i64_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_i64,
                             &input_i64_other, sizeof(input_i64_other));


    /* f32 */
    const float input_f32_forbidden =
        registry_tests_constrained_forbidden_values_f32.constraints.float32.forbidden_values[0];
    const float input_f32_other =
        registry_tests_constrained_forbidden_values_f32.constraints.float32.forbidden_values[0] +
        1.0;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_f32,
                                 &input_f32_forbidden, sizeof(input_f32_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_f32,
                             &input_f32_other, sizeof(input_f32_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);


    /* f64 */
    const double input_f64_forbidden =
        registry_tests_constrained_forbidden_values_f64.constraints.float64.forbidden_values[0];
    const double input_f64_other =
        registry_tests_constrained_forbidden_values_f64.constraints.float64.forbidden_values[0] +
        1.0;

    res_forbidden = registry_set(&test_constrained_forbidden_values_instance_1,
                                 &registry_tests_constrained_forbidden_values_f64,
                                 &input_f64_forbidden, sizeof(input_f64_forbidden));

    res_other = registry_set(&test_constrained_forbidden_values_instance_1,
                             &registry_tests_constrained_forbidden_values_f64,
                             &input_f64_other, sizeof(input_f64_other));

    TEST_ASSERT_EQUAL_INT(-EINVAL, res_forbidden);
    TEST_ASSERT_EQUAL_INT(0, res_other);
}

Test *tests_registry_get_set_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(tests_registry_min_values),
        new_TestFixture(tests_registry_zero_values),
        new_TestFixture(tests_registry_max_values),
        new_TestFixture(tests_registry_constraints_min_max),
        new_TestFixture(tests_registry_constraints_allowed_values),
        new_TestFixture(tests_registry_constraints_forbidden_values),
    };

    EMB_UNIT_TESTCALLER(registry_tests, test_registry_setup, test_registry_teardown, fixtures);

    return (Test *)&registry_tests;
}

#endif

/** @} */
