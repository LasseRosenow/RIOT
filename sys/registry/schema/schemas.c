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
 *
 * @}
 */

#include <stdio.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "registry.h"

#include "registry/schemas.h"

/* BASE GET FUNCTION */

int _registry_schema_get_buf(const registry_schema_t *schema, const registry_instance_t *instance,
                             const registry_schema_item_t *parameter, const void **buf,
                             size_t *buf_len)
{
    registry_value_t value;

    int res = registry_get(schema, instance, parameter, &value);

    *buf = value.buf;

    if (buf_len != NULL) {
        *buf_len = value.buf_len;
    }

    return res;
}

/* BASE SET FUNCTION */

int _registry_schema_set_buf(const registry_schema_t *schema, const registry_instance_t *instance,
                             const registry_schema_item_t *parameter, const registry_type_t type,
                             const void *val, size_t val_len)
{
    const registry_value_t value = {
        .buf = val,
        .buf_len = val_len,
        .type = type,
    };

    int res = registry_set(schema, instance, parameter, &value);

    return res;
}
