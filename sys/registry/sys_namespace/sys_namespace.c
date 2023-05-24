/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_schema RIOT Registry Schema
 * @ingroup     sys
 * @brief       RIOT Registry Schema module providing common sys configuration schemas for the RIOT Registry sys module
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
#include "registry.h"

#include "registry/sys_namespace.h"
#include "registry/sys_namespace/util.h"

/* Sys namespace */

const registry_namespace_t registry_sys = {
    .name = "sys",
    .description = "Sys namespace",
    .schemas = (const registry_schema_t *[]) {
        &registry_sys_rgb_led,
    },
    .schemas_len = 1,
};

const registry_typed_sys_t registry_typed_sys = {
    .data = &registry_sys,
    .rgb_led = &registry_typed_sys_rgb_led,
};
