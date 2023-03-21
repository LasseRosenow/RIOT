/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_sys_rgb_led RIOT Registry Schema: RGB_LED
 * @ingroup     sys
 * @brief       RIOT Registry RGB_LED Schema representing the basic structure of an RGB LED
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "registry.h"
#include "registry/schema.h"
#include "registry/path.h"
#include "registry/schema/rgb_led.h"

/* Mapping */

static void mapping(const registry_id_t parameter_id, const registry_instance_t *instance,
                    void **val, size_t *val_len)
{
    registry_sys_rgb_led_instance_t *_instance =
        (registry_sys_rgb_led_instance_t *)instance->data;

    switch (parameter_id) {
    case REGISTRY_SYS_RGB_LED_RED:
        *val = &_instance->red;
        *val_len = sizeof(_instance->red);
        break;

    case REGISTRY_SYS_RGB_LED_GREEN:
        *val = &_instance->green;
        *val_len = sizeof(_instance->green);
        break;

    case REGISTRY_SYS_RGB_LED_BLUE:
        *val = &_instance->blue;
        *val_len = sizeof(_instance->blue);
        break;

    case REGISTRY_SYS_RGB_LED_BRIGHTNESSES_WHITE:
        *val = &_instance->white;
        *val_len = sizeof(_instance->white);
        break;

    case REGISTRY_SYS_RGB_LED_BRIGHTNESSES_YELLOW:
        *val = &_instance->yellow;
        *val_len = sizeof(_instance->yellow);
        break;
    }
}

/* Schema */

static int _get_red(const registry_instance_t *instance, uint8_t **value)
{
    return _registry_schema_util_get_buf(&registry_sys_rgb_led.data, instance,
                                         &registry_sys_rgb_led.red.data, (const void **)value,
                                         NULL);
}

static int _set_red(const registry_instance_t *instance, uint8_t value)
{
    return _registry_schema_util_set_buf(&registry_sys_rgb_led.data, instance,
                                         &registry_sys_rgb_led.red.data,
                                         registry_sys_rgb_led.red.data.type, (const void *)&value,
                                         sizeof(uint8_t));
}

const registry_sys_rgb_led_t registry_sys_rgb_led = {
    .data = {
        .id = 0,
        .name = "rgb_led",
        .description = "",
        .mapping = mapping,
        .items = (registry_schema_item_t *[]) {
            &registry_sys_rgb_led.red.data,
            &registry_sys_rgb_led.green.data,
            &registry_sys_rgb_led.blue.data,
            &registry_sys_rgb_led.brightnesses.data,
        },
        .items_len = 4,
    },
    .red = {
        .data = {
            .id = 0,
            .name = "red",
            .description = "",
            .type = REGISTRY_TYPE_UINT8,
            .items = NULL,
            .items_len = 0,
        },
        .get = _get_red,
        .set = _set_red,
    },
    .green = {
        .data = {
            .id = 1,
            .name = "green",
            .description = "",
            .type = REGISTRY_TYPE_UINT8,
            .items = NULL,
            .items_len = 0,
        },
        .get = _get_red,
    },
    .blue = {
        .data = {
            .id = 2,
            .name = "blue",
            .description = "",
            .type = REGISTRY_TYPE_UINT8,
            .items = NULL,
            .items_len = 0,
        },
        .get = _get_red,
    },
    .brightnesses = {
        .data = {
            .id = 3,
            .name = "brightnesses",
            .description = "",
            .type = REGISTRY_TYPE_GROUP,
            .items = (registry_schema_item_t *[]) {
                &registry_sys_rgb_led.brightnesses.white.data,
                &registry_sys_rgb_led.brightnesses.yellow.data,
            },
            .items_len = 2,
        },
        .white = {
            .data = {
                .id = 4,
                .name = "white",
                .description = "",
                .type = REGISTRY_TYPE_UINT8,
                .items = NULL,
                .items_len = 0,
            },
            .get = _get_red,
        },
        .yellow = {
            .data = {
                .id = 5,
                .name = "yellow",
                .description = "",
                .type = REGISTRY_TYPE_UINT8,
                .items = NULL,
                .items_len = 0,
            },
            .get = _get_red,
        },
    },
};
