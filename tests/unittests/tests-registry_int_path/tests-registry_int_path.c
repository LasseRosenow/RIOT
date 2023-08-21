/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    unittests
 * @brief       Unittests for the ``registry_int_path`` module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#include "embUnit/embUnit.h"
#include "tests-registry_int_path.h"

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
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/nested.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_NESTED) || IS_ACTIVE(DOXYGEN)

static registry_tests_nested_instance_t test_instance_data = {
    .parameter = 9,
    .group_parameter = 5,
};

static registry_instance_t test_instance = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_META_NAME)
    .name = "test-nested-parameter-test",
#endif /* CONFIG_REGISTRY_ENABLE_META_NAME */
    .data = &test_instance_data,
    .commit_cb = NULL,
};

static void test_setup(void)
{
    /* init registry */
    registry_init();

    /* add schema instances */
    registry_add_schema_instance(&registry_tests_nested, &test_instance);
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
static void tests_registry_from_group_or_parameter_int_path(void)
{
    registry_int_path_type_t path_type;
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;
    registry_group_t *group;
    registry_parameter_t *parameter;


    /* parameter */
    const registry_group_or_parameter_int_path_t parameter_path = {
        .namespace_id = registry_tests.id,
        .schema_id = registry_tests_nested.id,
        .instance_id = test_instance.id,
        .group_or_parameter_id = registry_tests_nested_group_parameter.id,
    };

    registry_from_group_or_parameter_int_path(&parameter_path, &path_type, &namespace, &schema,
                                              &instance, &group, &parameter);

    TEST_ASSERT_EQUAL_INT(path_type, REGISTRY_INT_PATH_TYPE_PARAMETER);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests, (int)namespace);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested, (int)schema);
    TEST_ASSERT_EQUAL_INT((int)&test_instance, (int)instance);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested_group_parameter, (int)parameter);


    /* group */
    const registry_group_or_parameter_int_path_t group_path = {
        .namespace_id = registry_tests.id,
        .schema_id = registry_tests_nested.id,
        .instance_id = test_instance.id,
        .group_or_parameter_id = registry_tests_nested_group.id,
    };

    registry_from_group_or_parameter_int_path(&group_path, &path_type, &namespace, &schema,
                                              &instance, &group, &parameter);

    TEST_ASSERT_EQUAL_INT(path_type, REGISTRY_INT_PATH_TYPE_GROUP);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests, (int)namespace);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested, (int)schema);
    TEST_ASSERT_EQUAL_INT((int)&test_instance, (int)instance);
    TEST_ASSERT_EQUAL_INT((int)&registry_tests_nested_group, (int)group);
}

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
        new_TestFixture(tests_registry_from_group_or_parameter_int_path),
        new_TestFixture(tests_registry_from_parameter_int_path),
        new_TestFixture(tests_registry_from_group_int_path),
        new_TestFixture(tests_registry_from_instance_int_path),
        new_TestFixture(tests_registry_from_schema_int_path),
        new_TestFixture(tests_registry_from_namespace_int_path),
    };

    EMB_UNIT_TESTCALLER(registry_int_path_tests, test_setup, NULL, fixtures);

    return (Test *)&registry_int_path_tests;
}

void tests_registry_int_path(void)
{
    TESTS_RUN(tests_registry_int_path_tests());
}

#endif

/** @} */
