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

void registry_schemas_init(void);

/* BASE GET FUNCTION */

inline int _registry_schema_get_buf(const registry_schema_t *schema,
                                    const registry_instance_t *instance, registry_id_t parameter_id,
                                    registry_type_t type, const void **val, size_t *val_len);

/* BASE SET FUNCTION */

inline int _registry_schema_set_buf(const registry_schema_t *schema,
                                    const registry_instance_t *instance, registry_id_t parameter_id,
                                    registry_type_t type, const void *val, size_t val_len);

/* BASE TYPES */

/* BASE COMMIT TYPE */



/* BASE PARAMETER TYPES */

typedef const struct {
    const registry_schema_item_t data;
    int (*get)(const registry_instance_t *instance, uint8_t **val);
    int (*set)(const registry_instance_t *instance, const uint8_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_uint8_t;

/* ------------- Registry sys namespace ------------- */

typedef const struct {
    const registry_namespace_t data;
    const registry_schema_rgb_led_t * const rgb_led;
} registry_sys_t;

extern const registry_sys_t registry_sys;

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_SCHEMAS_H */
/** @} */
