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

#if IS_USED(MODULE_REGISTRY_NAMESPACE_SYS_RGB_LED) || IS_ACTIVE(DOXYGEN)

/* Mapping */
static void mapping(const registry_resource_id_t parameter_id, const registry_instance_t *instance,
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
const registry_resource_t registry_sys_rgb_led_red = {
    .id = 0,
    .name = "red",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_sys_rgb_led_green = {
    .id = 1,
    .name = "green",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_sys_rgb_led_blue = {
    .id = 2,
    .name = "blue",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_sys_rgb_led_brightnesses_white = {
    .id = 4,
    .name = "white",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_sys_rgb_led_brightnesses_yellow = {
    .id = 5,
    .name = "yellow",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .resources = NULL,
    .resources_len = 0,
};

const registry_resource_t registry_sys_rgb_led_brightnesses = {
    .id = 3,
    .name = "brightnesses",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_GROUP,
    .resources = (const registry_resource_t *[]) {
        &registry_sys_rgb_led_brightnesses_white,
        &registry_sys_rgb_led_brightnesses_yellow,
    },
    .resources_len = 2,
};

const registry_schema_t registry_sys_rgb_led = {
    .id = 0,
    .name = "rgb_led",
    .description = "",
    .namespace = &registry_sys,
    .mapping = mapping,
    .resources = (const registry_resource_t *[]) {
        &registry_sys_rgb_led_red,
        &registry_sys_rgb_led_green,
        &registry_sys_rgb_led_blue,
        &registry_sys_rgb_led_brightnesses,
    },
    .resources_len = 4,
};

#endif
