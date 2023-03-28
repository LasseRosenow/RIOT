/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_sys_rgb_led RIOT Registry Schema: RGB_LED
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
#include "registry/path.h"
#include "registry/namespace_sys.h"

#include "registry/namespace_sys/rgb_led.h"

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
    return _registry_schema_util_get_buf(&registry_sys_rgb_led, instance,
                                         &registry_sys_rgb_led_red, (const void **)value,
                                         NULL);
}

static int _set_red(const registry_instance_t *instance, uint8_t value)
{
    return _registry_schema_util_set_buf(&registry_sys_rgb_led, instance,
                                         &registry_sys_rgb_led_red,
                                         registry_sys_rgb_led_red.type, (const void *)&value,
                                         sizeof(uint8_t));
}

const registry_schema_item_t registry_sys_rgb_led_red = {
    .id = 0,
    .name = "red",
    .description = "",
    .type = REGISTRY_TYPE_UINT8,
    .items = NULL,
    .items_len = 0,
};

const registry_schema_item_t registry_sys_rgb_led_green = {
    .id = 1,
    .name = "green",
    .description = "",
    .type = REGISTRY_TYPE_UINT8,
    .items = NULL,
    .items_len = 0,
};

const registry_schema_item_t registry_sys_rgb_led_blue = {
    .id = 2,
    .name = "blue",
    .description = "",
    .type = REGISTRY_TYPE_UINT8,
    .items = NULL,
    .items_len = 0,
};

const registry_schema_item_t registry_sys_rgb_led_brightnesses_white = {
    .id = 4,
    .name = "white",
    .description = "",
    .type = REGISTRY_TYPE_UINT8,
    .items = NULL,
    .items_len = 0,
};

const registry_schema_item_t registry_sys_rgb_led_brightnesses_yellow = {
    .id = 5,
    .name = "yellow",
    .description = "",
    .type = REGISTRY_TYPE_UINT8,
    .items = NULL,
    .items_len = 0,
};

const registry_schema_item_t registry_sys_rgb_led_brightnesses = {
    .id = 3,
    .name = "brightnesses",
    .description = "",
    .type = REGISTRY_TYPE_GROUP,
    .items = (registry_schema_item_t *[]) {
        &registry_sys_rgb_led_brightnesses_white,
        &registry_sys_rgb_led_brightnesses_yellow,
    },
    .items_len = 2,
};

const registry_schema_t registry_sys_rgb_led = {
    .id = 0,
    .name = "rgb_led",
    .description = "",
    .mapping = mapping,
    .items = (registry_schema_item_t *[]) {
        &registry_sys_rgb_led_red,
        &registry_sys_rgb_led_green,
        &registry_sys_rgb_led_blue,
        &registry_sys_rgb_led_brightnesses,
    },
    .items_len = 4,
};

const registry_typed_sys_rgb_led_t registry_typed_sys_rgb_led = {
    .data = &registry_sys_rgb_led,
    .red = {
        .data = &registry_sys_rgb_led_red,
        .get = _get_red,
        .set = _set_red,
    },
    .green = {
        .data = &registry_sys_rgb_led_green,
        .get = _get_red,
    },
    .blue = {
        .data = &registry_sys_rgb_led_blue,
        .get = _get_red,
    },
    .brightnesses = {
        .data = &registry_sys_rgb_led_brightnesses,
        .white = {
            .data = &registry_sys_rgb_led_brightnesses_white,
            .get = _get_red,
        },
        .yellow = {
            .data = &registry_sys_rgb_led_brightnesses_yellow,
            .get = _get_red,
        },
    },
};
