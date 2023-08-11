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
 * @brief       Unittests for registry_load
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

#define _TESTS_REGISTRY_LOAD_STORED_VALUE 60

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
    if (storage == &storage_test_instance) {
        uint8_t buf = _TESTS_REGISTRY_LOAD_STORED_VALUE;
        return load_cb(&test_nested_instance, &registry_tests_nested_parameter, &buf, sizeof(buf));
    }

    return -EINVAL;
}

static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_parameter_t *parameter,
                const registry_value_t *value)
{
    (void)storage;
    (void)instance;
    (void)parameter;
    (void)value;

    return 0;
}

REGISTRY_ADD_STORAGE_SOURCE(storage_test_instance);

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

static void tests_registry_load(void)
{
    /* check if the registry_load function gets the correct input values internally */
    TEST_ASSERT(registry_load() == 0);

    /* check if the load_cb sets the value to the registry */
    registry_value_t output;

    registry_get(&test_nested_instance, &registry_tests_nested_parameter, &output);
    TEST_ASSERT_EQUAL_INT(_TESTS_REGISTRY_LOAD_STORED_VALUE, *(uint8_t *)output.buf);
}

Test *tests_registry_load_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(tests_registry_load),
    };

    EMB_UNIT_TESTCALLER(registry_tests, test_registry_setup, test_registry_teardown, fixtures);

    return (Test *)&registry_tests;
}

#endif

/** @} */
