/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespaces RIOT Registry Namespaces
 * @ingroup     sys
 * @brief       RIOT Registry Namespaces module providing namespaces for the RIOT Registry
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <stdio.h>

#define ENABLE_DEBUG (0)
#include "debug.h"

#include "registry/namespaces.h"

const registry_sys_t registry_sys = {
    .data = {
        .id = 0,
        .name = "sys",
        .description = "Sys namespace",
        .items = {
            &registry_schema_rgb_led,
        },
        .items_len = 1,
    },
    .rgb_led = &registry_schema_rgb_led,
};
