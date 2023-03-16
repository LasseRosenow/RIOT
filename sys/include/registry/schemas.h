/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_schemas RIOT Registry Schemas
 * @ingroup     sys
 * @brief       RIOT Registry Schemas module providing common sys schemas for the RIOT Registry sys module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_SCHEMAS_H
#define REGISTRY_SCHEMAS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"
#include "registry/schema/rgb_led.h"

/* Sys namespace */

typedef const struct {
    const registry_namespace_t data;
    const registry_sys_rgb_led_t * const rgb_led;
} registry_sys_t;

extern const registry_sys_t registry_sys;

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_SCHEMAS_H */
/** @} */
