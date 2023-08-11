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
 * @brief       Unittests for registry_save
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
#include "registry/storage.h"

#include "tests-registry.h"
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/nested.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_NESTED) || IS_ACTIVE(DOXYGEN)

static registry_tests_nested_instance_t test_nested_instance_data = {
    .parameter = 9,
    .group_parameter = 5,
};

static registry_instance_t test_nested_instance = {
    .name = "test-nested-parameter-test",
    .data = &test_nested_instance_data,
    .commit_cb = NULL,
};

static int load(const registry_storage_instance_t *storage,
                const load_cb_t load_cb);
static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_parameter_t *parameter,
                const registry_value_t *value);

static registry_storage_t storage_test = {
    .load = load,
    .save = save,
};

static uint8_t storage_test_instance_data = 7;

static registry_storage_instance_t storage_test_instance = {
    .storage = &storage_test,
    .data = &storage_test_instance_data,
};

static int load(const registry_storage_instance_t *storage,
                const load_cb_t load_cb)
{
    (void)storage;
    (void)load_cb;

    return 0;
}

static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_parameter_t *parameter,
                const registry_value_t *value)
{
    if (storage == &storage_test_instance &&
        instance == &test_nested_instance &&
        parameter == &registry_tests_nested_group_parameter &&
        value->buf == &test_nested_instance_data.group_parameter &&
        value->buf_len == sizeof(uint8_t) &&
        value->type == REGISTRY_TYPE_UINT8) {

        return 0;
    }

    return -EINVAL;
}

REGISTRY_SET_STORAGE_DESTINATION(storage_test_instance);

static void test_registry_setup(void)
{
    /* init registry */
    registry_init();

    /* add schema instances */
    registry_add_schema_instance(&registry_tests_nested, &test_nested_instance);
}

static void test_registry_teardown(void)
{
}

static void tests_registry_save_parameter(void)
{
    TEST_ASSERT_EQUAL_INT(0, registry_save_parameter(&test_nested_instance,
                                                     &registry_tests_nested_group_parameter));
}

static void tests_registry_save_group(void)
{
    TEST_ASSERT_EQUAL_INT(0, registry_save_group(&test_nested_instance,
                                                 &registry_tests_nested_group));
}

static void tests_registry_save_instance(void)
{
    TEST_ASSERT_EQUAL_INT(0, registry_save_instance(&test_nested_instance));
}

static void tests_registry_save_schema(void)
{
    TEST_ASSERT_EQUAL_INT(0, registry_save_schema(&registry_tests_nested));
}

static void tests_registry_save_namespace(void)
{
    TEST_ASSERT_EQUAL_INT(0, registry_save_namespace(&registry_tests));
}

static void tests_registry_save_all(void)
{
    TEST_ASSERT_EQUAL_INT(0, registry_save());
}

Test *tests_registry_save_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(tests_registry_save_parameter),
        new_TestFixture(tests_registry_save_group),
        new_TestFixture(tests_registry_save_instance),
        new_TestFixture(tests_registry_save_schema),
        new_TestFixture(tests_registry_save_namespace),
        new_TestFixture(tests_registry_save_all),
    };

    EMB_UNIT_TESTCALLER(registry_tests, test_registry_setup, test_registry_teardown, fixtures);

    return (Test *)&registry_tests;
}

#endif

/** @} */
