/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    unittests
 * @brief       Unittests for the ``registry`` module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */
#ifndef TESTS_REGISTRY_H
#define TESTS_REGISTRY_H

#include "embUnit.h"

#ifdef __cplusplus
extern "C" {
#endif

int registry_tests_api_run(void);
int registry_tests_stack_run(void);

#ifdef __cplusplus
}
#endif

#endif /* TESTS_REGISTRY_H */
/** @} */
