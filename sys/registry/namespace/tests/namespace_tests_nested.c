/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_tests_nested RIOT Registry Schema: Nested
 * @ingroup     sys
 * @brief       RIOT Registry Nested Schema representing different nesting level of a configuration schema
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "registry.h"
#include "registry/namespace/tests.h"
#include "registry/namespace/tests/nested.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_NESTED) || IS_ACTIVE(DOXYGEN)

/* Mapping */
static void mapping(const registry_parameter_id_t parameter_id, const registry_instance_t *instance,
                    void **val, size_t *val_len)
{
    registry_tests_nested_instance_t *_instance =
        (registry_tests_nested_instance_t *)instance->data;

    switch (parameter_id) {
    case REGISTRY_TESTS_NESTED_PARAMETER:
        *val = &_instance->parameter;
        *val_len = sizeof(_instance->parameter);
        break;

    case REGISTRY_TESTS_NESTED_GROUP_PARAMETER:
        *val = &_instance->group_parameter;
        *val_len = sizeof(_instance->group_parameter);
        break;
    }
}

/* Schema */
const registry_parameter_t registry_tests_nested_parameter = {
    .id = REGISTRY_TESTS_NESTED_PARAMETER,
    .name = "parameter",
    .description = "",
    .schema = &registry_tests_nested,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
    },
};

const registry_group_t registry_tests_nested_group = {
    .id = REGISTRY_TESTS_NESTED_GROUP,
    .name = "group",
    .description = "",
    .schema = &registry_tests_nested,
    .groups = NULL,
    .groups_len = 0,
    .parameters = (const registry_parameter_t *[]) {
        &registry_tests_nested_group_parameter,
    },
    .parameters_len = 1,
};

const registry_parameter_t registry_tests_nested_group_parameter = {
    .id = REGISTRY_TESTS_NESTED_GROUP_PARAMETER,
    .name = "parameter",
    .description = "",
    .schema = &registry_tests_nested,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
    },
};

registry_schema_t registry_tests_nested = {
    .id = REGISTRY_TESTS_NESTED,
    .name = "nested",
    .description = "",
    .namespace = &registry_tests,
    .mapping = mapping,
    .groups = (const registry_group_t *[]) {
        &registry_tests_nested_group,
    },
    .groups_len = 1,
    .parameters = (const registry_parameter_t *[]) {
        &registry_tests_nested_parameter,
    },
    .parameters_len = 1,
};

#endif
