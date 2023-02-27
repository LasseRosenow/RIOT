/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_cli RIOT Registry Schemas
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

void registry_schemas_init(void);

/* Schema IDs */
typedef enum {
    REGISTRY_SCHEMA_RGB_LED = 0,
} registry_schema_id_t;

/* ------------- Schemas ------------- */

/* RGB-LED */
extern registry_schema_t registry_schema_rgb_led;

typedef struct {
    clist_node_t node;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} registry_schema_rgb_led_t;

typedef enum {
    REGISTRY_SCHEMA_RGB_LED_RED,
    REGISTRY_SCHEMA_RGB_LED_GREEN,
    REGISTRY_SCHEMA_RGB_LED_BLUE,
} registry_schema_rgb_led_indices_t;

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* REGISTRY_SCHEMAS_H */
