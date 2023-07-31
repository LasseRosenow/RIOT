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
#include "registry.h"

#include "registry/namespace/tests.h"
#include "registry/namespace/tests/full.h"

/* Tests namespace */

registry_namespace_t registry_tests = {
    .name = "tests",
    .description = "Tests namespace",
    .schemas = (const registry_schema_t *[]) {
#if IS_USED(MODULE_REGISTRY_NAMESPACE_TESTS_FULL)
        &registry_tests_full,
#endif
    },
    .schemas_len = 1,
};
