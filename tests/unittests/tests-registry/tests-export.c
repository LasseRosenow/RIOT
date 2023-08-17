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
 * @brief       Unittests for registry_export
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
#include "registry/namespace/tests/nested.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_NESTED) || IS_ACTIVE(DOXYGEN)

static bool successful = false;

static registry_tests_nested_instance_t test_nested_instance_data = {
    .parameter = 9,
    .group_parameter = 5,
};

static registry_instance_t test_nested_instance_1 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_META_NAME)
    .name = "test-nested-parameter-test",
#endif /* CONFIG_REGISTRY_ENABLE_META_NAME */
    .data = &test_nested_instance_data,
    .commit_cb = NULL,
};

static int export_parameter_cb(const registry_export_cb_data_t *data,
                               const registry_export_cb_data_type_t data_type,
                               const void *context)
{
    (void)context;

    if (data_type == REGISTRY_EXPORT_PARAMETER && data != NULL &&
        data->parameter.data->id == *(registry_parameter_id_t *)context &&
        data->parameter.instance == &test_nested_instance_1) {
        successful = true;
    }

    return 0;
}

static int export_group_cb(const registry_export_cb_data_t *data,
                           const registry_export_cb_data_type_t data_type,
                           const void *context)
{
    (void)context;

    if (data_type == REGISTRY_EXPORT_GROUP && data != NULL &&
        data->group->id == *(registry_group_id_t *)context) {
        successful = true;
    }

    return 0;
}

static int export_instance_cb(const registry_export_cb_data_t *data,
                              const registry_export_cb_data_type_t data_type,
                              const void *context)
{
    (void)context;

    if (data_type == REGISTRY_EXPORT_INSTANCE && data != NULL &&
        data->instance == &test_nested_instance_1) {
        successful = true;
    }

    return 0;
}

static int export_schema_cb(const registry_export_cb_data_t *data,
                            const registry_export_cb_data_type_t data_type,
                            const void *context)
{
    (void)context;

    if (data_type == REGISTRY_EXPORT_SCHEMA && data != NULL &&
        data->schema == &registry_tests_nested) {
        successful = true;
    }

    return 0;
}

static int export_namespace_cb(const registry_export_cb_data_t *data,
                               const registry_export_cb_data_type_t data_type,
                               const void *context)
{
    (void)context;

    if (data_type == REGISTRY_EXPORT_NAMESPACE && data != NULL &&
        data->namespace == &registry_tests) {
        successful = true;
    }

    return 0;
}

static void test_registry_setup(void)
{
    /* init registry */
    registry_init();

    /* add schema instances */
    registry_add_schema_instance(&registry_tests_nested, &test_nested_instance_1);
}

static void test_registry_teardown(void)
{
}

static void tests_registry_export_parameter(void)
{
    const registry_parameter_id_t parameter_id = REGISTRY_TESTS_NESTED_PARAMETER;

    successful = false;
    registry_export_parameter(&test_nested_instance_1, &registry_tests_nested_parameter,
                              &export_parameter_cb, &parameter_id);
    TEST_ASSERT(successful);
}

static void tests_registry_export_group(void)
{
    /* check if group gets exported */
    const registry_group_id_t group_id = REGISTRY_TESTS_NESTED_GROUP;

    successful = false;
    registry_export_group(&test_nested_instance_1, &registry_tests_nested_group, &export_group_cb,
                          0, &group_id);
    TEST_ASSERT(successful);


    /* check that siblings get NOT exported */
    const registry_parameter_id_t sibling_parameter_id = REGISTRY_TESTS_NESTED_PARAMETER;
    successful = false;
    registry_export_group(&test_nested_instance_1, &registry_tests_nested_group,
                          &export_parameter_cb, 0, &sibling_parameter_id);
    TEST_ASSERT_EQUAL_INT(false, successful);


    /* check if children get exported */
    const registry_parameter_id_t child_parameter_id = REGISTRY_TESTS_NESTED_GROUP_PARAMETER;

    /* recursion_depth 0 => infinite => parameter gets exported */
    successful = false;
    registry_export_group(&test_nested_instance_1, &registry_tests_nested_group,
                          &export_parameter_cb, 0, &child_parameter_id);
    TEST_ASSERT(successful);

    /* recursion_depth 1 => only group => parameter gets NOT exported */
    successful = false;
    registry_export_group(&test_nested_instance_1, &registry_tests_nested_group,
                          &export_parameter_cb, 1, &child_parameter_id);
    TEST_ASSERT_EQUAL_INT(false, successful);

    /* recursion_depth 2 => group + 1 level more => parameter gets exported */
    successful = false;
    registry_export_group(&test_nested_instance_1, &registry_tests_nested_group,
                          &export_parameter_cb, 2, &child_parameter_id);
    TEST_ASSERT(successful);
}

static void tests_registry_export_instance(void)
{
    /* check if instance gets exported */
    successful = false;
    registry_export_instance(&test_nested_instance_1, &export_instance_cb, 0, NULL);
    TEST_ASSERT(successful);


    /* check if group gets exported */
    const registry_group_id_t group_id = REGISTRY_TESTS_NESTED_GROUP;

    successful = false;
    registry_export_instance(&test_nested_instance_1, &export_group_cb, 0, &group_id);
    TEST_ASSERT(successful);


    /* check if parameter get exported */
    const registry_parameter_id_t child_parameter_id = REGISTRY_TESTS_NESTED_GROUP_PARAMETER;

    /* recursion_depth 0 => infinite => parameter gets exported */
    successful = false;
    registry_export_instance(&test_nested_instance_1, &export_parameter_cb, 0, &child_parameter_id);
    TEST_ASSERT(successful);

    /* recursion_depth 2 => only instance and group => parameter gets NOT exported */
    successful = false;
    registry_export_instance(&test_nested_instance_1, &export_parameter_cb, 2, &child_parameter_id);
    TEST_ASSERT_EQUAL_INT(false, successful);

    /* recursion_depth 3 => instance, group and parameter => parameter gets exported */
    successful = false;
    registry_export_instance(&test_nested_instance_1, &export_parameter_cb, 3, &child_parameter_id);
    TEST_ASSERT(successful);
}

static void tests_registry_export_schema(void)
{
    /* check if schema gets exported */
    successful = false;
    registry_export_schema(&registry_tests_nested, &export_schema_cb, 0, NULL);
    TEST_ASSERT(successful);

    /* check if instance gets exported */
    successful = false;
    registry_export_schema(&registry_tests_nested, &export_instance_cb, 0, NULL);
    TEST_ASSERT(successful);


    /* check if group gets exported */
    const registry_group_id_t group_id = REGISTRY_TESTS_NESTED_GROUP;

    successful = false;
    registry_export_schema(&registry_tests_nested, &export_group_cb, 0, &group_id);
    TEST_ASSERT(successful);


    /* check if parameter get exported */
    const registry_parameter_id_t child_parameter_id = REGISTRY_TESTS_NESTED_GROUP_PARAMETER;

    /* recursion_depth 0 => infinite => parameter gets exported */
    successful = false;
    registry_export_schema(&registry_tests_nested, &export_parameter_cb, 0, &child_parameter_id);
    TEST_ASSERT(successful);

    /* recursion_depth 3 => only schema, instance and group => parameter gets NOT exported */
    successful = false;
    registry_export_schema(&registry_tests_nested, &export_parameter_cb, 3, &child_parameter_id);
    TEST_ASSERT_EQUAL_INT(false, successful);

    /* recursion_depth 4 => schema, instance, group and parameter => parameter gets exported */
    successful = false;
    registry_export_schema(&registry_tests_nested, &export_parameter_cb, 4, &child_parameter_id);
    TEST_ASSERT(successful);
}

static void tests_registry_export_namespace(void)
{
    /* check if namespace gets exported */
    successful = false;
    registry_export_namespace(&registry_tests, &export_namespace_cb, 0, NULL);
    TEST_ASSERT(successful);

    /* check if schema gets exported */
    successful = false;
    registry_export_namespace(&registry_tests, &export_schema_cb, 0, NULL);
    TEST_ASSERT(successful);

    /* check if instance gets exported */
    successful = false;
    registry_export_namespace(&registry_tests, &export_instance_cb, 0, NULL);
    TEST_ASSERT(successful);


    /* check if group gets exported */
    const registry_group_id_t group_id = REGISTRY_TESTS_NESTED_GROUP;

    successful = false;
    registry_export_namespace(&registry_tests, &export_group_cb, 0, &group_id);
    TEST_ASSERT(successful);


    /* check if parameter get exported */
    const registry_parameter_id_t child_parameter_id = REGISTRY_TESTS_NESTED_GROUP_PARAMETER;

    /* recursion_depth 0 => infinite => parameter gets exported */
    successful = false;
    registry_export_namespace(&registry_tests, &export_parameter_cb, 0, &child_parameter_id);
    TEST_ASSERT(successful);

    /* recursion_depth 4 => only namespace, schema, instance and group => parameter gets NOT exported */
    successful = false;
    registry_export_namespace(&registry_tests, &export_parameter_cb, 4, &child_parameter_id);
    TEST_ASSERT_EQUAL_INT(false, successful);

    /* recursion_depth 5 => namespace, schema, instance, group and parameter => parameter gets exported */
    successful = false;
    registry_export_namespace(&registry_tests, &export_parameter_cb, 5, &child_parameter_id);
    TEST_ASSERT(successful);
}

static void tests_registry_export_all(void)
{
    /* check if namespace gets exported */
    successful = false;
    registry_export(&export_namespace_cb, 0, NULL);
    TEST_ASSERT(successful);

    /* check if schema gets exported */
    successful = false;
    registry_export( &export_schema_cb, 0, NULL);
    TEST_ASSERT(successful);

    /* check if instance gets exported */
    successful = false;
    registry_export( &export_instance_cb, 0, NULL);
    TEST_ASSERT(successful);


    /* check if group gets exported */
    const registry_group_id_t group_id = REGISTRY_TESTS_NESTED_GROUP;

    successful = false;
    registry_export( &export_group_cb, 0, &group_id);
    TEST_ASSERT(successful);


    /* check if parameter get exported */
    const registry_parameter_id_t child_parameter_id = REGISTRY_TESTS_NESTED_GROUP_PARAMETER;

    /* recursion_depth 0 => infinite => parameter gets exported */
    successful = false;
    registry_export( &export_parameter_cb, 0, &child_parameter_id);
    TEST_ASSERT(successful);

    /* recursion_depth 4 => only namespace, schema, instance and group => parameter gets NOT exported */
    successful = false;
    registry_export(&export_parameter_cb, 4, &child_parameter_id);
    TEST_ASSERT_EQUAL_INT(false, successful);

    /* recursion_depth 5 => namespace, schema, instance, group and parameter => parameter gets exported */
    successful = false;
    registry_export(&export_parameter_cb, 5, &child_parameter_id);
    TEST_ASSERT(successful);
}

Test *tests_registry_export_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(tests_registry_export_parameter),
        new_TestFixture(tests_registry_export_group),
        new_TestFixture(tests_registry_export_instance),
        new_TestFixture(tests_registry_export_schema),
        new_TestFixture(tests_registry_export_namespace),
        new_TestFixture(tests_registry_export_all),
    };

    EMB_UNIT_TESTCALLER(registry_tests, test_registry_setup, test_registry_teardown, fixtures);

    return (Test *)&registry_tests;
}

#endif

/** @} */
