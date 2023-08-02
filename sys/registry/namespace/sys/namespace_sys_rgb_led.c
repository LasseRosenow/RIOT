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
#include "registry/namespace/sys.h"

#include "registry/namespace/sys/rgb_led.h"

#if IS_USED(MODULE_REGISTRY_NAMESPACE_SYS_RGB_LED) || IS_ACTIVE(DOXYGEN)

/* Mapping */
static void mapping(const registry_parameter_id_t parameter_id, const registry_instance_t *instance,
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
const registry_parameter_t registry_sys_rgb_led_red = {
    .id = REGISTRY_SYS_RGB_LED_RED,
    .name = "red",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_sys_rgb_led_green = {
    .id = REGISTRY_SYS_RGB_LED_GREEN,
    .name = "green",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_sys_rgb_led_blue = {
    .id = REGISTRY_SYS_RGB_LED_BLUE,
    .name = "blue",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_sys_rgb_led_brightnesses_white = {
    .id = REGISTRY_SYS_RGB_LED_BRIGHTNESSES_WHITE,
    .name = "white",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_parameter_t registry_sys_rgb_led_brightnesses_yellow = {
    .id = REGISTRY_SYS_RGB_LED_BRIGHTNESSES_YELLOW,
    .name = "yellow",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .type = REGISTRY_TYPE_UINT8,
    .constraints.uint8 = {
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .allowed_values = NULL,
        .allowed_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_ALLOWED_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK) || IS_ACTIVE(DOXYGEN)
        .forbidden_values = NULL,
        .forbidden_values_len = 0,
#endif /* CONFIG_REGISTRY_ENABLE_FORBIDDEN_VALUES_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .min_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MIN_VALUE_CHECK */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK) || IS_ACTIVE(DOXYGEN)
        .max_value = NULL,
#endif /* CONFIG_REGISTRY_ENABLE_MAX_VALUE_CHECK */
    },
};

const registry_group_t registry_sys_rgb_led_brightnesses = {
    .id = REGISTRY_SYS_RGB_LED_BRIGHTNESSES,
    .name = "brightnesses",
    .description = "",
    .schema = &registry_sys_rgb_led,
    .groups = NULL,
    .groups_len = 0,
    .parameters = (const registry_parameter_t *[]) {
        &registry_sys_rgb_led_brightnesses_white,
        &registry_sys_rgb_led_brightnesses_yellow,
    },
    .parameters_len = 2,
};

registry_schema_t registry_sys_rgb_led = {
    .id = 0,
    .name = "rgb_led",
    .description = "",
    .namespace = &registry_sys,
    .mapping = mapping,
    .groups = (const registry_group_t *[]) {
        &registry_sys_rgb_led_brightnesses,
    },
    .groups_len = 1,
    .parameters = (const registry_parameter_t *[]) {
        &registry_sys_rgb_led_red,
        &registry_sys_rgb_led_green,
        &registry_sys_rgb_led_blue,
    },
    .parameters_len = 3,
};

#endif
