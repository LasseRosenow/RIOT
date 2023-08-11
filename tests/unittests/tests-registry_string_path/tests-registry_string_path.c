/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    unittests
 * @brief       Unittests for the ``registry_string_path`` module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#include "embUnit/embUnit.h"
#include "tests-registry_string_path.h"

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
#include "registry/string_path.h"
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/nested.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_NESTED) || IS_ACTIVE(DOXYGEN)

static registry_tests_nested_instance_t test_instance_data = {
    .parameter = 9,
    .group_parameter = 5,
};

static registry_instance_t test_instance = {
    .name = "instance-1",
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

/* to string_path */
static void tests_registry_to_parameter_string_path(void)
{
    int size = registry_to_parameter_string_path(&test_instance,
                                                 &registry_tests_nested_group_parameter, NULL);
    char path[size + 1];

    registry_to_parameter_string_path(&test_instance, &registry_tests_nested_group_parameter, path);

    TEST_ASSERT_EQUAL_STRING("/tests/nested/instance-1/group/parameter", path);
}

static void tests_registry_to_group_string_path(void)
{
    int size = registry_to_group_string_path(&test_instance, &registry_tests_nested_group, NULL);
    char path[size + 1];

    registry_to_group_string_path(&test_instance, &registry_tests_nested_group, path);

    TEST_ASSERT_EQUAL_STRING("/tests/nested/instance-1/group", path);
}

static void tests_registry_to_instance_string_path(void)
{
    int size = registry_to_instance_string_path(&test_instance, NULL);
    char path[size + 1];

    registry_to_instance_string_path(&test_instance, path);

    TEST_ASSERT_EQUAL_STRING("/tests/nested/instance-1", path);
}

static void tests_registry_to_schema_string_path(void)
{
    int size = registry_to_schema_string_path(&registry_tests_nested, NULL);
    char path[size + 1];

    registry_to_schema_string_path(&registry_tests_nested, path);

    TEST_ASSERT_EQUAL_STRING("/tests/nested", path);
}

static void tests_registry_to_namespace_string_path(void)
{
    int size = registry_to_namespace_string_path(&registry_tests, NULL);
    char path[size + 1];

    registry_to_namespace_string_path(&registry_tests, path);

    TEST_ASSERT_EQUAL_STRING("/tests", path);
}

/* from string_path */
static void tests_registry_from_group_or_parameter_string_path(void)
{
    registry_string_path_type_t path_type;
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;
    registry_group_t *group;
    registry_parameter_t *parameter;

    /* group */
    registry_from_group_or_parameter_string_path("/tests/nested/instance-1/group",
                                                 &path_type, &namespace, &schema, &instance, &group,
                                                 &parameter);

    TEST_ASSERT_EQUAL_INT(REGISTRY_STRING_PATH_TYPE_GROUP, path_type);
    TEST_ASSERT_EQUAL_STRING("tests", namespace->name);
    TEST_ASSERT_EQUAL_STRING("nested", schema->name);
    TEST_ASSERT_EQUAL_STRING("instance-1", instance->name);
    TEST_ASSERT_EQUAL_STRING("group", group->name);

    /* parameter */
    registry_from_group_or_parameter_string_path("/tests/nested/instance-1/group/parameter",
                                                 &path_type, &namespace, &schema, &instance, &group,
                                                 &parameter);

    TEST_ASSERT_EQUAL_INT(REGISTRY_STRING_PATH_TYPE_PARAMETER, path_type);
    TEST_ASSERT_EQUAL_STRING("tests", namespace->name);
    TEST_ASSERT_EQUAL_STRING("nested", schema->name);
    TEST_ASSERT_EQUAL_STRING("instance-1", instance->name);
    TEST_ASSERT_EQUAL_STRING("parameter", parameter->name);
}

static void tests_registry_from_parameter_string_path(void)
{
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;
    registry_parameter_t *parameter;

    registry_from_parameter_string_path("/tests/nested/instance-1/group/parameter", &namespace,
                                        &schema, &instance, &parameter);

    TEST_ASSERT_EQUAL_STRING("tests", namespace->name);
    TEST_ASSERT_EQUAL_STRING("nested", schema->name);
    TEST_ASSERT_EQUAL_STRING("instance-1", instance->name);
    TEST_ASSERT_EQUAL_STRING("parameter", parameter->name);
}

static void tests_registry_from_group_string_path(void)
{
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;
    registry_group_t *group;

    registry_from_group_string_path("/tests/nested/instance-1/group", &namespace, &schema,
                                    &instance, &group);

    TEST_ASSERT_EQUAL_STRING("tests", namespace->name);
    TEST_ASSERT_EQUAL_STRING("nested", schema->name);
    TEST_ASSERT_EQUAL_STRING("instance-1", instance->name);
    TEST_ASSERT_EQUAL_STRING("group", group->name);
}

static void tests_registry_from_instance_string_path(void)
{
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;

    registry_from_instance_string_path("/tests/nested/instance-1", &namespace, &schema, &instance);

    TEST_ASSERT_EQUAL_STRING("tests", namespace->name);
    TEST_ASSERT_EQUAL_STRING("nested", schema->name);
    TEST_ASSERT_EQUAL_STRING("instance-1", instance->name);
}

static void tests_registry_from_schema_string_path(void)
{
    registry_namespace_t *namespace;
    registry_schema_t *schema;

    registry_from_schema_string_path("/tests/nested", &namespace, &schema);

    TEST_ASSERT_EQUAL_STRING("tests", namespace->name);
    TEST_ASSERT_EQUAL_STRING("nested", schema->name);
}

static void tests_registry_from_namespace_string_path(void)
{
    registry_namespace_t *namespace;

    registry_from_namespace_string_path("/tests", &namespace);

    TEST_ASSERT_EQUAL_STRING("tests", namespace->name);
}

Test *tests_registry_string_path_tests(void)
{
    // TODO enable remaining tests
    (void)tests_registry_from_group_or_parameter_string_path;
    (void)tests_registry_from_parameter_string_path;
    (void)tests_registry_from_group_string_path;
    EMB_UNIT_TESTFIXTURES(fixtures) {
        /* to string_path */
        new_TestFixture(tests_registry_to_parameter_string_path),
        new_TestFixture(tests_registry_to_group_string_path),
        new_TestFixture(tests_registry_to_instance_string_path),
        new_TestFixture(tests_registry_to_schema_string_path),
        new_TestFixture(tests_registry_to_namespace_string_path),
        /* from string_path */
        // new_TestFixture(tests_registry_from_group_or_parameter_string_path),
        // new_TestFixture(tests_registry_from_parameter_string_path),
        // new_TestFixture(tests_registry_from_group_string_path),
        new_TestFixture(tests_registry_from_instance_string_path),
        new_TestFixture(tests_registry_from_schema_string_path),
        new_TestFixture(tests_registry_from_namespace_string_path),
    };

    EMB_UNIT_TESTCALLER(registry_string_path_tests, test_setup, NULL, fixtures);

    return (Test *)&registry_string_path_tests;
}

void tests_registry_string_path(void)
{
    TESTS_RUN(tests_registry_string_path_tests());
}

#endif

/** @} */
