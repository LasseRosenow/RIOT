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

/* BASE GET FUNCTIONS */

int registry_schema_get_opaque(const registry_instance_t *instance, void **val, size_t *val_len);

/* BASE SET FUNCTIONS */

int registry_schema_set_opaque(const registry_instance_t *instance, const void *val,
                               const size_t val_len);
/* BASE TYPES */

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, void **val, size_t *val_len);
    int (*set)(const registry_instance_t *instance, const void *val, const size_t val_len);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_opaque_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, char **val, size_t *val_len);
    int (*set)(const registry_instance_t *instance, const char *val, const size_t val_len);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_string_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, bool **val);
    int (*set)(const registry_instance_t *instance, const bool val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_bool_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, uint8_t **val);
    int (*set)(const registry_instance_t *instance, const uint8_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_uint8_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, uint16_t **val);
    int (*set)(const registry_instance_t *instance, const uint16_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_uint16_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, uint32_t **val);
    int (*set)(const registry_instance_t *instance, const uint32_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_uint32_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, uint64_t **val);
    int (*set)(const registry_instance_t *instance, const uint64_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_uint64_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, int8_t **val);
    int (*set)(const registry_instance_t *instance, const int8_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_int8_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, int16_t **val);
    int (*set)(const registry_instance_t *instance, const int16_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_int16_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, int32_t **val);
    int (*set)(const registry_instance_t *instance, const int32_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_int32_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, int64_t **val);
    int (*set)(const registry_instance_t *instance, const int64_t val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_int64_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, float **val);
    int (*set)(const registry_instance_t *instance, const float val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_float32_t;

typedef const struct {
    const registry_schema_item_data_t data;
    int (*get)(const registry_instance_t *instance, double **val);
    int (*set)(const registry_instance_t *instance, const double val);
    int (*commit)(void);
    int (*export)(const registry_export_cb_t *export_cb, const void *context);
} registry_parameter_float64_t;


/* ------------- Schema IDs ------------- */

typedef enum {
    REGISTRY_SCHEMA_RGB_LED = 0,
} registry_schema_id_t;

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
