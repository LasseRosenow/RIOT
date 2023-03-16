/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_schema_rgb_led RIOT Registry Schema: RGB_LED
 * @ingroup     sys
 * @brief       RIOT Registry RGB_LED Schema representing the basic structure of an RGB LED
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_SCHEMA_RGB_LED_H
#define REGISTRY_SCHEMA_RGB_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry/schemas.h"

/* RGB-LED */
// #if IS_USED(MODULE_REGISTRY_SCHEMA_RGB_LED) || IS_ACTIVE(DOXYGEN)
typedef const struct {
    const registry_schema_t data;
    const registry_parameter_uint8_t red;
    const registry_parameter_uint8_t green;
    const registry_parameter_uint8_t blue;
    const struct {
        const registry_schema_item_t data;
        int (*commit)(void);
        int (*export)(const registry_export_cb_t *export_cb, const void *context);
        const registry_parameter_uint8_t white;
        const registry_parameter_uint8_t yellow;
    } brightnesses;
} registry_sys_rgb_led_t;

extern const registry_sys_rgb_led_t registry_sys_rgb_led;

typedef struct {
    clist_node_t node;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;
    uint8_t yellow;
} registry_sys_rgb_led_instance_t;

typedef const enum {
    REGISTRY_SYS_RGB_LED_RED,
    REGISTRY_SYS_RGB_LED_GREEN,
    REGISTRY_SYS_RGB_LED_BLUE,
    REGISTRY_SYS_RGB_LED_BRIGHTNESSES,
    REGISTRY_SYS_RGB_LED_BRIGHTNESSES_WHITE,
    REGISTRY_SYS_RGB_LED_BRIGHTNESSES_YELLOW,
} registry_sys_rgb_led_indices_t;

// #endif

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_SCHEMA_RGB_LED_H */
/** @} */
