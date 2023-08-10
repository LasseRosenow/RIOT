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
 * @brief       Unittests for registry_to_int_path, registry_from_int_path
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
#include "registry/int_path.h"

#include "tests-registry.h"
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/nested.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_NESTED) || IS_ACTIVE(DOXYGEN)

static registry_tests_nested_instance_t test_instance_data = {
    .parameter = 9,
    .group_parameter = 5,
};

static registry_instance_t test_instance = {
    .name = "test-nested-parameter-test",
    .data = &test_instance_data,
    .commit_cb = NULL,
};

static void test_registry_setup(void)
{
    /* init registry */
    registry_init();

    /* add schema instances */
    registry_register_schema_instance(&registry_tests_nested, &test_instance);
}

static void test_registry_teardown(void)
{
}

/* to int_path */
static void tests_registry_to_parameter_int_path(void)
{
    registry_parameter_int_path_t path = registry_to_parameter_int_path(&test_instance,
                                                                        &registry_tests_nested_parameter);

    TEST_ASSERT_EQUAL_INT(registry_tests.id, path.namespace_id);
    TEST_ASSERT_EQUAL_INT(registry_tests_nested.id, path.schema_id);
    TEST_ASSERT_EQUAL_INT(test_instance.id, path.instance_id);
    TEST_ASSERT_EQUAL_INT(registry_tests_nested_parameter.id, path.parameter_id);
}

static void tests_registry_to_group_int_path(void)
{
    registry_group_int_path_t path = registry_to_group_int_path(&test_instance,
                                                                &registry_tests_nested_group);

    TEST_ASSERT_EQUAL_INT(registry_tests.id, path.namespace_id);
    TEST_ASSERT_EQUAL_INT(registry_tests_nested.id, path.schema_id);
    TEST_ASSERT_EQUAL_INT(test_instance.id, path.instance_id);
    TEST_ASSERT_EQUAL_INT(registry_tests_nested_group.id, path.group_id);
}

static void tests_registry_to_instance_int_path(void)
{
    registry_instance_int_path_t path = registry_to_instance_int_path(&test_instance);

    TEST_ASSERT_EQUAL_INT(registry_tests.id, path.namespace_id);
    TEST_ASSERT_EQUAL_INT(registry_tests_nested.id, path.schema_id);
    TEST_ASSERT_EQUAL_INT(test_instance.id, path.instance_id);
}

static void tests_registry_to_schema_int_path(void)
{
    registry_schema_int_path_t path = registry_to_schema_int_path(&registry_tests_nested);

    TEST_ASSERT_EQUAL_INT(registry_tests.id, path.namespace_id);
    TEST_ASSERT_EQUAL_INT(registry_tests_nested.id, path.schema_id);
}

static void tests_registry_to_namespace_int_path(void)
{
    registry_namespace_int_path_t path = registry_to_namespace_int_path(&registry_tests);

    TEST_ASSERT_EQUAL_INT(registry_tests.id, path.namespace_id);
}

/* from int_path */
static void tests_registry_from_parameter_int_path(void)
{
    const registry_parameter_int_path_t path = {
        .namespace_id = registry_tests.id,
        .schema_id = registry_tests_nested.id,
        .instance_id = test_instance.id,
        .parameter_id = registry_tests_nested_group_parameter.id,
    };
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;
    registry_parameter_t *parameter;

    registry_from_parameter_int_path(&path, &namespace, &schema, &instance, &parameter);

    TEST_ASSERT_EQUAL_INT((int)&registry_tests, (int)namespace);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested, (int)schema);
    TEST_ASSERT_EQUAL_INT((int)&test_instance, (int)instance);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested_group_parameter, (int)parameter);
}

static void tests_registry_from_group_int_path(void)
{
    const registry_group_int_path_t path = {
        .namespace_id = registry_tests.id,
        .schema_id = registry_tests_nested.id,
        .instance_id = test_instance.id,
        .group_id = registry_tests_nested_group.id,
    };
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;
    registry_group_t *group;

    registry_from_group_int_path(&path, &namespace, &schema, &instance, &group);

    TEST_ASSERT_EQUAL_INT((int)&registry_tests, (int)namespace);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested, (int)schema);
    TEST_ASSERT_EQUAL_INT((int)&test_instance, (int)instance);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested_group, (int)group);
}

static void tests_registry_from_instance_int_path(void)
{
    const registry_instance_int_path_t path = {
        .namespace_id = registry_tests.id,
        .schema_id = registry_tests_nested.id,
        .instance_id = test_instance.id,
    };
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;

    registry_from_instance_int_path(&path, &namespace, &schema, &instance);

    TEST_ASSERT_EQUAL_INT((int)&registry_tests, (int)namespace);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested, (int)schema);
    TEST_ASSERT_EQUAL_INT((int)&test_instance, (int)instance);
}

static void tests_registry_from_schema_int_path(void)
{
    const registry_schema_int_path_t path = {
        .namespace_id = registry_tests.id,
        .schema_id = registry_tests_nested.id,
    };
    registry_namespace_t *namespace;
    registry_schema_t *schema;

    registry_from_schema_int_path(&path, &namespace, &schema);

    TEST_ASSERT_EQUAL_INT((int)&registry_tests, (int)namespace);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested, (int)schema);
}

static void tests_registry_from_namespace_int_path(void)
{
    const registry_namespace_int_path_t path = {
        .namespace_id = registry_tests.id,
    };
    registry_namespace_t *namespace;

    registry_from_namespace_int_path(&path, &namespace);

    TEST_ASSERT_EQUAL_INT((int)&registry_tests, (int)namespace);
}

Test *tests_registry_int_path_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        /* to int_path */
        new_TestFixture(tests_registry_to_parameter_int_path),
        new_TestFixture(tests_registry_to_group_int_path),
        new_TestFixture(tests_registry_to_instance_int_path),
        new_TestFixture(tests_registry_to_schema_int_path),
        new_TestFixture(tests_registry_to_namespace_int_path),
        /* from int_path */
        new_TestFixture(tests_registry_from_parameter_int_path),
        new_TestFixture(tests_registry_from_group_int_path),
        new_TestFixture(tests_registry_from_instance_int_path),
        new_TestFixture(tests_registry_from_schema_int_path),
        new_TestFixture(tests_registry_from_namespace_int_path),
    };

    EMB_UNIT_TESTCALLER(registry_tests, test_registry_setup, test_registry_teardown, fixtures);

    return (Test *)&registry_tests;
}

#endif

/** @} */
