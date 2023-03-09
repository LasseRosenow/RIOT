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

void registry_schemas_init(void);

/* Schema IDs */
typedef enum {
    REGISTRY_SCHEMA_RGB_LED = 0,
} registry_schema_id_t;

/* ------------- Schemas ------------- */

/* RGB-LED */
// #if IS_USED(MODULE_REGISTRY_SCHEMA_RGB_LED) || IS_ACTIVE(DOXYGEN)
typedef const struct {
    const registry_schema_data_t data;
    const struct {
        const registry_schema_item_data_t data;
        int (*get)(uint8_t **val, size_t *val_len);
    } red;
    const struct {
        const registry_schema_item_data_t data;
        int (*get)(uint8_t **val, size_t *val_len);
    } green;
    const struct {
        const registry_schema_item_data_t data;
        int (*get)(uint8_t **val, size_t *val_len);
    } blue;
    const struct {
        const registry_schema_item_data_t data;
        const struct {
            const registry_schema_item_data_t data;
            int (*get)(uint8_t **val, size_t *val_len);
        } white;
        const struct {
            const registry_schema_item_data_t data;
            int (*get)(uint8_t **val, size_t *val_len);
        } yellow;
    } brightnesses;
} registry_schema_rgb_led_t;

extern const registry_schema_rgb_led_t registry_schema_rgb_led;

typedef struct {
    clist_node_t node;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;
    uint8_t yellow;
} registry_schema_rgb_led_instance_t;

typedef enum {
    REGISTRY_SCHEMA_RGB_LED_RED,
    REGISTRY_SCHEMA_RGB_LED_GREEN,
    REGISTRY_SCHEMA_RGB_LED_BLUE,
    REGISTRY_SCHEMA_RGB_LED_BRIGHTNESSES,
    REGISTRY_SCHEMA_RGB_LED_BRIGHTNESSES_WHITE,
    REGISTRY_SCHEMA_RGB_LED_BRIGHTNESSES_YELLOW,
} registry_schema_rgb_led_indices_t;

// #endif

/* ------------- Registry sys namespace ------------- */

typedef const struct {
    const registry_namespace_data_t data;
    const registry_schema_rgb_led_t rgb_led;
} registry_sys_t;

extern const registry_sys_t registry_sys;

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_SCHEMAS_H */
/** @} */
