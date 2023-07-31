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

#include "tests_registry.h"
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/full.h"

#define FLOAT_MAX_CHAR_COUNT ((FLT_MAX_10_EXP + 1) + 1 + 1 + 6)     // (FLT_MAX_10_EXP + 1) + sign + dot + 6 decimal places
#define DOUBLE_MAX_CHAR_COUNT ((DBL_MAX_10_EXP + 1) + 1 + 1 + 6)    // (DBL_MAX_10_EXP + 1) + sign + dot + 6 decimal places

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
    registry_value_t output_value;

    /* string */
    const char input_string[] = "";

    registry_set(&test_instance_1, &registry_tests_full_string, &input_string);
    registry_get(&test_instance_1, &registry_tests_full_string, &output_value);

    TEST_ASSERT_EQUAL_STRING("", (char *)output_value.buf);


    // /* bool */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_BOOL);

    // registry_set_bool(path, false);

    // const bool *bool_out;

    // registry_get_bool(path, &bool_out);

    // TEST_ASSERT_EQUAL_INT(false, *bool_out);


    // /* u8 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_U8);

    // registry_set_uint8(path, 0);

    // const uint8_t *output_u8;

    // registry_get_uint8(path, &output_u8);

    // TEST_ASSERT_EQUAL_INT(0, *output_u8);


    // /* u16 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_U16);

    // registry_set_uint16(path, 0);

    // const uint16_t *output_u16;

    // registry_get_uint16(path, &output_u16);

    // TEST_ASSERT_EQUAL_INT(0, *output_u16);


    // /* u32 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_U32);

    // registry_set_uint32(path, 0);

    // const uint32_t *output_u32;

    // registry_get_uint32(path, &output_u32);

    // TEST_ASSERT_EQUAL_INT(0, *output_u32);


    // /* u64 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_U64);
    // registry_set_uint64(path, 0);
    // const uint64_t *output_u64;
    // registry_get_uint64(path, &output_u64);
    // TEST_ASSERT_EQUAL_INT(0, *output_u64);


    // /* i8 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_I8);
    // registry_set_int8(path, INT8_MIN);
    // const int8_t *output_i8;
    // registry_get_int8(path, &output_i8);
    // TEST_ASSERT_EQUAL_INT(INT8_MIN, *output_i8);


    // /* i16 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_I16);
    // registry_set_int16(path, INT16_MIN);
    // const int16_t *output_i16;
    // registry_get_int16(path, &output_i16);
    // TEST_ASSERT_EQUAL_INT(INT16_MIN, *output_i16);


    // /* i32 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_I32);
    // registry_set_int32(path, INT32_MIN);
    // const int32_t *output_i32;
    // registry_get_int32(path, &output_i32);
    // TEST_ASSERT_EQUAL_INT(INT32_MIN, *output_i32);


    // /* i64 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_I64);
    // registry_set_int64(path, INT64_MIN);
    // const int64_t *output_i64;
    // registry_get_int64(path, &output_i64);
    // TEST_ASSERT_EQUAL_INT(INT64_MIN, *output_i64);


    // /* f32 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_F32);
    // registry_set_float32(path, -FLT_MAX);
    // const float *output_f32;
    // registry_get_float32(path, &output_f32);
    // char input_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    // char output_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    // sprintf(input_f32_string, "%f", -FLT_MAX);
    // sprintf(output_f32_string, "%f", *output_f32);
    // TEST_ASSERT_EQUAL_STRING(input_f32_string, output_f32_string);


    // /* f64 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_F64);
    // registry_set_float64(path, -DBL_MAX);
    // const double *output_f64;
    // registry_get_float64(path, &output_f64);
    // char input_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    // char output_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    // sprintf(input_f64_string, "%f", -DBL_MAX);
    // sprintf(output_f64_string, "%f", *output_f64);
    // TEST_ASSERT_EQUAL_STRING(input_f64_string, output_f64_string);
}

static void tests_registry_max_values(void)
{
    // /* string */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_STRING);
    // char input_string[50] = { 0 };

    // for (size_t i = 0; i < 50 - 1; i++) {
    //     input_string[i] = 'a';
    // }

    // registry_set_string(path, input_string);

    // const char *ouput_string;

    // registry_get_string(path, &ouput_string, NULL);

    // TEST_ASSERT_EQUAL_STRING(input_string, ouput_string);


    // /* bool */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_BOOL);

    // registry_set_bool(path, true);

    // const bool *output_bool;

    // registry_get_bool(path, &output_bool);

    // TEST_ASSERT_EQUAL_INT(true, *output_bool);


    // /* u8 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_U8);

    // registry_set_uint8(path, UINT8_MAX);

    // const uint8_t *output_u8;

    // registry_get_uint8(path, &output_u8);

    // TEST_ASSERT_EQUAL_INT(UINT8_MAX, *output_u8);


    // /* u16 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_U16);

    // registry_set_uint16(path, UINT16_MAX);

    // const uint16_t *output_u16;

    // registry_get_uint16(path, &output_u16);

    // TEST_ASSERT_EQUAL_INT(UINT16_MAX, *output_u16);


    // /* u32 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_U32);

    // registry_set_uint32(path, UINT32_MAX);

    // const uint32_t *output_u32;

    // registry_get_uint32(path, &output_u32);

    // TEST_ASSERT_EQUAL_INT(UINT32_MAX, *output_u32);


    // /* u64 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_U64);
    // registry_set_uint64(path, UINT64_MAX);
    // const uint64_t *output_u64;
    // registry_get_uint64(path, &output_u64);
    // TEST_ASSERT_EQUAL_INT(UINT64_MAX, *output_u64);


    // /* i8 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_I8);
    // registry_set_int8(path, INT8_MAX);
    // const int8_t *output_i8;
    // registry_get_int8(path, &output_i8);
    // TEST_ASSERT_EQUAL_INT(INT8_MAX, *output_i8);


    // /* i16 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_I16);
    // registry_set_int16(path, INT16_MAX);
    // const int16_t *output_i16;
    // registry_get_int16(path, &output_i16);
    // TEST_ASSERT_EQUAL_INT(INT16_MAX, *output_i16);


    // /* i32 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_I32);
    // registry_set_int32(path, INT32_MAX);
    // const int32_t *output_i32;
    // registry_get_int32(path, &output_i32);
    // TEST_ASSERT_EQUAL_INT(INT32_MAX, *output_i32);


    // /* i64 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_I64);
    // registry_set_int64(path, INT64_MAX);
    // const int64_t *output_i64;
    // registry_get_int64(path, &output_i64);
    // TEST_ASSERT_EQUAL_INT(INT64_MAX, *output_i64);


    // /* f32 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_F32);
    // registry_set_float32(path, FLT_MAX);
    // const float *output_f32;
    // registry_get_float32(path, &output_f32);
    // char input_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    // char output_f32_string[FLOAT_MAX_CHAR_COUNT + 1] = { 0 };
    // sprintf(input_f32_string, "%f", FLT_MAX);
    // sprintf(output_f32_string, "%f", *output_f32);
    // TEST_ASSERT_EQUAL_STRING(input_f32_string, output_f32_string);


    // /* f64 */
    // path = REGISTRY_PATH_SYS(REGISTRY_SCHEMA_FULL_EXAMPLE, 0, REGISTRY_SCHEMA_FULL_EXAMPLE_F64);
    // registry_set_float64(path, DBL_MAX);
    // const double *output_f64;
    // registry_get_float64(path, &output_f64);
    // char input_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    // char output_f64_string[DOUBLE_MAX_CHAR_COUNT + 1] = { 0 };
    // sprintf(input_f64_string, "%f", DBL_MAX);
    // sprintf(output_f64_string, "%f", *output_f64);
    // TEST_ASSERT_EQUAL_STRING(input_f64_string, output_f64_string);
}

static Test *tests_registry(void)
{
    (void)tests_registry_min_values;
    (void)tests_registry_max_values;

    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(tests_registry_min_values),
        new_TestFixture(tests_registry_max_values),
    };

    EMB_UNIT_TESTCALLER(registry_tests, test_registry_setup, test_registry_teardown, fixtures);

    return (Test *)&registry_tests;
}

int tests_registry_get_and_set_run(void)
{
    TESTS_START();
    TESTS_RUN(tests_registry());
    TESTS_END();
    return 0;
}

/** @} */
