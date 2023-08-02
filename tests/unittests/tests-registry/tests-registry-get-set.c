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

#define FLOAT_MAX_CHAR_COUNT ((FLT_MAX_10_EXP + 1) + 1 + 1 + 6)     // (FLT_MAX_10_EXP + 1) + sign + dot + 6 decimal places
#define DOUBLE_MAX_CHAR_COUNT ((DBL_MAX_10_EXP + 1) + 1 + 1 + 6)    // (DBL_MAX_10_EXP + 1) + sign + dot + 6 decimal places

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_FULL) || IS_ACTIVE(DOXYGEN)

static registry_tests_full_instance_t test_instance_1_data = {
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

int test_instance_0_commit_cb(const registry_commit_cb_scope_t scope,
                              const registry_resource_id_t *resource_id,
                              const void *context)
{
    (void)scope;
    (void)resource_id;
    (void)context;

    return 0;
}

static registry_instance_t test_instance_1 = {
    .name = "test-1",
    .data = &test_instance_1_data,
    .commit_cb = &test_instance_0_commit_cb,
};

static void test_registry_setup(void)
{
    /* register tests namespace */
    registry_register_namespace(&registry_tests);

    /* add schema instances */
    registry_register_schema_instance(&registry_tests_full, &test_instance_1);
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

    registry_set(&test_instance_1, &registry_tests_full_opaque, &input_opaque,
                 sizeof(input_opaque));
    registry_get(&test_instance_1, &registry_tests_full_opaque, &output);

    TEST_ASSERT_EQUAL_INT(input_opaque.value,
                          ((registry_tests_full_instance_opaque_t *)output.buf)->value);

    /* string */
    const char input_string[] = "";

    registry_set(&test_instance_1, &registry_tests_full_string, input_string, sizeof(input_string));
    registry_get(&test_instance_1, &registry_tests_full_string, &output);

    TEST_ASSERT_EQUAL_STRING("", (char *)output.buf);


    /* bool */
    const bool input_bool = false;

    registry_set(&test_instance_1, &registry_tests_full_boolean, &input_bool, sizeof(input_bool));
    registry_get(&test_instance_1, &registry_tests_full_boolean, &output);

    TEST_ASSERT_EQUAL_INT(input_bool, *(bool *)output.buf);


    /* u8 */
    const uint8_t input_u8 = 0;

    registry_set(&test_instance_1, &registry_tests_full_u8, &input_u8, sizeof(input_u8));
    registry_get(&test_instance_1, &registry_tests_full_u8, &output);

    TEST_ASSERT_EQUAL_INT(input_u8, *(uint8_t *)output.buf);


    /* u16 */
    const uint16_t input_u16 = 0;

    registry_set(&test_instance_1, &registry_tests_full_u16, &input_u16, sizeof(input_u16));
    registry_get(&test_instance_1, &registry_tests_full_u16, &output);

    TEST_ASSERT_EQUAL_INT(input_u16, *(uint16_t *)output.buf);


    /* u32 */
    const uint32_t input_u32 = 0;

    registry_set(&test_instance_1, &registry_tests_full_u32, &input_u32, sizeof(input_u32));
    registry_get(&test_instance_1, &registry_tests_full_u32, &output);

    TEST_ASSERT_EQUAL_INT(input_u32, *(uint32_t *)output.buf);


    /* u64 */
    const uint64_t input_u64 = 0;

    registry_set(&test_instance_1, &registry_tests_full_u64, &input_u64, sizeof(input_u64));
    registry_get(&test_instance_1, &registry_tests_full_u64, &output);

    TEST_ASSERT_EQUAL_INT(input_u64, *(uint64_t *)output.buf);


    /* i8 */
    const int8_t input_i8 = INT8_MIN;

    registry_set(&test_instance_1, &registry_tests_full_i8, &input_i8, sizeof(input_i8));
    registry_get(&test_instance_1, &registry_tests_full_i8, &output);

    TEST_ASSERT_EQUAL_INT(input_i8, *(int8_t *)output.buf);


    /* i16 */
    const int16_t input_i16 = INT16_MIN;

    registry_set(&test_instance_1, &registry_tests_full_i16, &input_i16, sizeof(input_i16));
    registry_get(&test_instance_1, &registry_tests_full_i16, &output);

    TEST_ASSERT_EQUAL_INT(input_i16, *(int16_t *)output.buf);


    /* i32 */
    const int32_t input_i32 = INT32_MIN;

    registry_set(&test_instance_1, &registry_tests_full_i32, &input_i32, sizeof(input_i32));
    registry_get(&test_instance_1, &registry_tests_full_i32, &output);

    TEST_ASSERT_EQUAL_INT(input_i32, *(int32_t *)output.buf);


    /* i64 */
    const int64_t input_i64 = INT64_MIN;

    registry_set(&test_instance_1, &registry_tests_full_i64, &input_i64, sizeof(input_i64));
    registry_get(&test_instance_1, &registry_tests_full_i64, &output);

    TEST_ASSERT_EQUAL_INT(input_i64, *(int64_t *)output.buf);


    /* f32 */
    const float input_f32 = FLT_MIN;

    registry_set(&test_instance_1, &registry_tests_full_f32, &input_f32, sizeof(input_f32));
    registry_get(&test_instance_1, &registry_tests_full_f32, &output);

    char input_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f32_string, "%f", input_f32);
    sprintf(output_f32_string, "%f", *(float *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f32_string, output_f32_string);


    /* f64 */
    const double input_f64 = DBL_MIN;

    registry_set(&test_instance_1, &registry_tests_full_f64, &input_f64, sizeof(input_f64));
    registry_get(&test_instance_1, &registry_tests_full_f64, &output);

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

    registry_set(&test_instance_1, &registry_tests_full_opaque, &input_opaque,
                 sizeof(input_opaque));
    registry_get(&test_instance_1, &registry_tests_full_opaque, &output);

    TEST_ASSERT_EQUAL_INT(input_opaque.value,
                          ((registry_tests_full_instance_opaque_t *)output.buf)->value);


    /* string */
    const char input_string[] = "";

    registry_set(&test_instance_1, &registry_tests_full_string, input_string, sizeof(input_string));
    registry_get(&test_instance_1, &registry_tests_full_string, &output);

    TEST_ASSERT_EQUAL_STRING("", (char *)output.buf);


    /* bool */
    const bool input_bool = 0;

    registry_set(&test_instance_1, &registry_tests_full_boolean, &input_bool, sizeof(input_bool));
    registry_get(&test_instance_1, &registry_tests_full_boolean, &output);

    TEST_ASSERT_EQUAL_INT(input_bool, *(bool *)output.buf);


    /* u8 */
    const uint8_t input_u8 = 0;

    registry_set(&test_instance_1, &registry_tests_full_u8, &input_u8, sizeof(input_u8));
    registry_get(&test_instance_1, &registry_tests_full_u8, &output);

    TEST_ASSERT_EQUAL_INT(input_u8, *(uint8_t *)output.buf);


    /* u16 */
    const uint16_t input_u16 = 0;

    registry_set(&test_instance_1, &registry_tests_full_u16, &input_u16, sizeof(input_u16));
    registry_get(&test_instance_1, &registry_tests_full_u16, &output);

    TEST_ASSERT_EQUAL_INT(input_u16, *(uint16_t *)output.buf);


    /* u32 */
    const uint32_t input_u32 = 0;

    registry_set(&test_instance_1, &registry_tests_full_u32, &input_u32, sizeof(input_u32));
    registry_get(&test_instance_1, &registry_tests_full_u32, &output);

    TEST_ASSERT_EQUAL_INT(input_u32, *(uint32_t *)output.buf);


    /* u64 */
    const uint64_t input_u64 = 0;

    registry_set(&test_instance_1, &registry_tests_full_u64, &input_u64, sizeof(input_u64));
    registry_get(&test_instance_1, &registry_tests_full_u64, &output);

    TEST_ASSERT_EQUAL_INT(input_u64, *(uint64_t *)output.buf);


    /* i8 */
    const int8_t input_i8 = 0;

    registry_set(&test_instance_1, &registry_tests_full_i8, &input_i8, sizeof(input_i8));
    registry_get(&test_instance_1, &registry_tests_full_i8, &output);

    TEST_ASSERT_EQUAL_INT(input_i8, *(int8_t *)output.buf);


    /* i16 */
    const int16_t input_i16 = 0;

    registry_set(&test_instance_1, &registry_tests_full_i16, &input_i16, sizeof(input_i16));
    registry_get(&test_instance_1, &registry_tests_full_i16, &output);

    TEST_ASSERT_EQUAL_INT(input_i16, *(int16_t *)output.buf);


    /* i32 */
    const int32_t input_i32 = 0;

    registry_set(&test_instance_1, &registry_tests_full_i32, &input_i32, sizeof(input_i32));
    registry_get(&test_instance_1, &registry_tests_full_i32, &output);

    TEST_ASSERT_EQUAL_INT(input_i32, *(int32_t *)output.buf);


    /* i64 */
    const int64_t input_i64 = 0;

    registry_set(&test_instance_1, &registry_tests_full_i64, &input_i64, sizeof(input_i64));
    registry_get(&test_instance_1, &registry_tests_full_i64, &output);

    TEST_ASSERT_EQUAL_INT(input_i64, *(int64_t *)output.buf);


    /* f32 */
    const float input_f32 = 0.0;

    registry_set(&test_instance_1, &registry_tests_full_f32, &input_f32, sizeof(input_f32));
    registry_get(&test_instance_1, &registry_tests_full_f32, &output);

    char input_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f32_string, "%f", input_f32);
    sprintf(output_f32_string, "%f", *(float *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f32_string, output_f32_string);


    /* f64 */
    const double input_f64 = 0.0;

    registry_set(&test_instance_1, &registry_tests_full_f64, &input_f64, sizeof(input_f64));
    registry_get(&test_instance_1, &registry_tests_full_f64, &output);

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

    registry_set(&test_instance_1, &registry_tests_full_opaque, &input_opaque,
                 sizeof(input_opaque));
    registry_get(&test_instance_1, &registry_tests_full_opaque, &output);

    TEST_ASSERT_EQUAL_INT(input_opaque.value,
                          ((registry_tests_full_instance_opaque_t *)output.buf)->value);


    /* string */
    char input_string[sizeof(test_instance_1_data.string)] = { 0 };

    for (size_t i = 0; i < sizeof(input_string) - 1; i++) {
        input_string[i] = 'a';
    }

    registry_set(&test_instance_1, &registry_tests_full_string, input_string, sizeof(input_string));
    registry_get(&test_instance_1, &registry_tests_full_string, &output);

    TEST_ASSERT_EQUAL_STRING(input_string, (char *)output.buf);


    /* bool */
    const bool input_bool = true;

    registry_set(&test_instance_1, &registry_tests_full_boolean, &input_bool, sizeof(input_bool));
    registry_get(&test_instance_1, &registry_tests_full_boolean, &output);

    TEST_ASSERT_EQUAL_INT(input_bool, *(bool *)output.buf);


    /* u8 */
    const uint8_t input_u8 = UINT8_MAX;

    registry_set(&test_instance_1, &registry_tests_full_u8, &input_u8, sizeof(input_u8));
    registry_get(&test_instance_1, &registry_tests_full_u8, &output);

    TEST_ASSERT_EQUAL_INT(input_u8, *(uint8_t *)output.buf);


    /* u16 */
    const uint16_t input_u16 = UINT16_MAX;

    registry_set(&test_instance_1, &registry_tests_full_u16, &input_u16, sizeof(input_u16));
    registry_get(&test_instance_1, &registry_tests_full_u16, &output);

    TEST_ASSERT_EQUAL_INT(input_u16, *(uint16_t *)output.buf);


    /* u32 */
    const uint32_t input_u32 = UINT32_MAX;

    registry_set(&test_instance_1, &registry_tests_full_u32, &input_u32, sizeof(input_u32));
    registry_get(&test_instance_1, &registry_tests_full_u32, &output);

    TEST_ASSERT_EQUAL_INT(input_u32, *(uint32_t *)output.buf);


    /* u64 */
    const uint64_t input_u64 = UINT64_MAX;

    registry_set(&test_instance_1, &registry_tests_full_u64, &input_u64, sizeof(input_u64));
    registry_get(&test_instance_1, &registry_tests_full_u64, &output);

    TEST_ASSERT_EQUAL_INT(input_u64, *(uint64_t *)output.buf);


    /* i8 */
    const int8_t input_i8 = INT8_MAX;

    registry_set(&test_instance_1, &registry_tests_full_i8, &input_i8, sizeof(input_i8));
    registry_get(&test_instance_1, &registry_tests_full_i8, &output);

    TEST_ASSERT_EQUAL_INT(input_i8, *(int8_t *)output.buf);


    /* i16 */
    const int16_t input_i16 = INT16_MAX;

    registry_set(&test_instance_1, &registry_tests_full_i16, &input_i16, sizeof(input_i16));
    registry_get(&test_instance_1, &registry_tests_full_i16, &output);

    TEST_ASSERT_EQUAL_INT(input_i16, *(int16_t *)output.buf);


    /* i32 */
    const int32_t input_i32 = INT32_MAX;

    registry_set(&test_instance_1, &registry_tests_full_i32, &input_i32, sizeof(input_i32));
    registry_get(&test_instance_1, &registry_tests_full_i32, &output);

    TEST_ASSERT_EQUAL_INT(input_i32, *(int32_t *)output.buf);


    /* i64 */
    const int64_t input_i64 = INT64_MAX;

    registry_set(&test_instance_1, &registry_tests_full_i64, &input_i64, sizeof(input_i64));
    registry_get(&test_instance_1, &registry_tests_full_i64, &output);

    TEST_ASSERT_EQUAL_INT(input_i64, *(int64_t *)output.buf);


    /* f32 */
    const float input_f32 = FLT_MAX;

    registry_set(&test_instance_1, &registry_tests_full_f32, &input_f32, sizeof(input_f32));
    registry_get(&test_instance_1, &registry_tests_full_f32, &output);

    char input_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f32_string, "%f", input_f32);
    sprintf(output_f32_string, "%f", *(float *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f32_string, output_f32_string);


    /* f64 */
    const double input_f64 = DBL_MAX;

    registry_set(&test_instance_1, &registry_tests_full_f64, &input_f64, sizeof(input_f64));
    registry_get(&test_instance_1, &registry_tests_full_f64, &output);

    char input_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    char output_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    sprintf(input_f64_string, "%f", input_f64);
    sprintf(output_f64_string, "%f", *(double *)output.buf);

    TEST_ASSERT_EQUAL_STRING(input_f64_string, output_f64_string);
}

Test *tests_registry_get_set_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(tests_registry_min_values),
        new_TestFixture(tests_registry_zero_values),
        new_TestFixture(tests_registry_max_values),
    };

    EMB_UNIT_TESTCALLER(registry_tests, test_registry_setup, test_registry_teardown, fixtures);

    return (Test *)&registry_tests;
}

#endif

/** @} */
