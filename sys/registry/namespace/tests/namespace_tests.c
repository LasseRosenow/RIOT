/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_tests RIOT Registry Tests Namespace
 * @ingroup     tests
 * @brief       RIOT Registry Namespace Tests module providing common tests configuration schemas for the RIOT Registry sys module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#include <stdio.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "registry.h"

#include "registry/namespace/tests.h"
#include "registry/namespace/tests/full.h"
#include "registry/namespace/tests/constrained_min_max.h"
#include "registry/namespace/tests/constrained_allowed_values.h"
#include "registry/namespace/tests/constrained_forbidden_values.h"
#include "registry/namespace/tests/nested.h"

static const registry_schema_t *_schemas[] = {
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_FULL)
    &registry_tests_full,
#endif
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_CONSTRAINED_MIN_MAX)
    &registry_tests_constrained_min_max,
#endif
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_CONSTRAINED_ALLOWED_VALUES)
    &registry_tests_constrained_allowed_values,
#endif
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_CONSTRAINED_FORBIDDEN_VALUES)
    &registry_tests_constrained_forbidden_values,
#endif
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_NESTED)
    &registry_tests_nested,
#endif
};

registry_namespace_t registry_tests = {
    .name = "tests",
    .description = "Tests namespace",
    .schemas = _schemas,
    .schemas_len = ARRAY_SIZE(_schemas),
};

REGISTRY_ADD_NAMESPACE(tests, registry_tests);
