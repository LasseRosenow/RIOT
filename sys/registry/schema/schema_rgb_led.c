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
#include "registry/schemas.h"
#include "registry/path.h"
#include "registry/schema/rgb_led.h"

/* Mapping */

static void mapping(const registry_id_t param_id, const registry_instance_t *instance,
                    void **val,
                    size_t *val_len)
{
    registry_schema_rgb_led_instance_t *_instance =
        (registry_schema_rgb_led_instance_t *)instance->data;

    switch (param_id) {
    case REGISTRY_SCHEMA_RGB_LED_RED:
        *val = &_instance->red;
        *val_len = sizeof(_instance->red);
        break;

    case REGISTRY_SCHEMA_RGB_LED_GREEN:
        *val = &_instance->green;
        *val_len = sizeof(_instance->green);
        break;

    case REGISTRY_SCHEMA_RGB_LED_BLUE:
        *val = &_instance->blue;
        *val_len = sizeof(_instance->blue);
        break;

    case REGISTRY_SCHEMA_RGB_LED_BRIGHTNESSES_WHITE:
        *val = &_instance->white;
        *val_len = sizeof(_instance->white);
        break;

    case REGISTRY_SCHEMA_RGB_LED_BRIGHTNESSES_YELLOW:
        *val = &_instance->yellow;
        *val_len = sizeof(_instance->yellow);
        break;
    }
}

/* Schema */

registry_schema_rgb_led_t registry_schema_rgb_led = {
    .data = {
        .id = REGISTRY_SCHEMA_RGB_LED,
        .name = "rgb_led",
        .description = "",
        .mapping = mapping,
    },
    .red = {
        .data = {
            .id = REGISTRY_SCHEMA_RGB_LED_RED,
            .name = "red",
            .description = "",
            .type = REGISTRY_TYPE_UINT8,
        },
        .get = registry_get_uint8,
    },
    .green = {
        .data = {
            .id = REGISTRY_SCHEMA_RGB_LED_GREEN,
            .name = "green",
            .description = "",
            .type = REGISTRY_TYPE_UINT8,
        },
        .get = registry_get_uint8,
    },
    .blue = {
        .data = {
            .id = REGISTRY_SCHEMA_RGB_LED_BLUE,
            .name = "blue",
            .description = "",
            .type = REGISTRY_TYPE_UINT8,
        },
        .get = registry_get_uint8,
    },
    .brightnesses = {
        .data = {
            .id = REGISTRY_SCHEMA_RGB_LED_BRIGHTNESSES,
            .name = "brightnesses",
            .description = "",
            .type = REGISTRY_TYPE_GROUP,
        },
        .white = {
            .data = {
                .id = REGISTRY_SCHEMA_RGB_LED_BRIGHTNESSES_WHITE,
                .name = "white",
                .description = "",
                .type = REGISTRY_TYPE_UINT8,
            },
            .get = registry_get_uint8,
        },
        .yellow = {
            .data = {
                .id = REGISTRY_SCHEMA_RGB_LED_BRIGHTNESSES_YELLOW,
                .name = "yellow",
                .description = "",
                .type = REGISTRY_TYPE_UINT8,
            },
            .get = registry_get_uint8,
        },
    },
};

registry_path_schema_t registry_path_schema_rgb_led = {
    .data = &registry_schema_rgb_led.data,
    .items = (registry_path_schema_item_t[]) {
        {
            .data = &registry_schema_rgb_led.red.data,
            .items = NULL,
            .items_len = 0,
        },
        {
            .data = &registry_schema_rgb_led.green.data,
            .items = NULL,
            .items_len = 0,
        },
        {
            .data = &registry_schema_rgb_led.blue.data,
            .items = ((void *)0),
            .items_len = 0,
        },
        {
            .data = &registry_schema_rgb_led.brightnesses.data,
            .items = (registry_path_schema_item_t[]) {
                {
                    .data = &registry_schema_rgb_led.brightnesses.white.data,
                    .items = NULL,
                    .items_len = 0,
                },
                {
                    .data = &registry_schema_rgb_led.brightnesses.yellow.data,
                    .items = NULL,
                    .items_len = 0,
                },
            },
            .items_len = 2,
        },
    },
    .items_len = 4,
};
