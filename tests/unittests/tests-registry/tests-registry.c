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
 * @brief       Unittest entry point for the Registry test group
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#include "embUnit/embUnit.h"

#include "tests-registry.h"

Test *tests_registry_get_set_tests(void);
Test *tests_registry_commit_tests(void);
Test *tests_registry_export_tests(void);
Test *tests_registry_load_tests(void);
Test *tests_registry_save_tests(void);
Test *tests_registry_int_path_tests(void);

void tests_registry(void)
{
    TESTS_RUN(tests_registry_get_set_tests());
    TESTS_RUN(tests_registry_commit_tests());
    TESTS_RUN(tests_registry_export_tests());
    TESTS_RUN(tests_registry_load_tests());
    TESTS_RUN(tests_registry_save_tests());
    TESTS_RUN(tests_registry_int_path_tests());
}
/** @} */
