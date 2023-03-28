/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_sys_namespace_util RIOT Registry Sys Namespace utilities
 * @ingroup     sys
 * @brief       RIOT Registry Sys Namespace Util module providing utility functions
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_SYS_NAMESPACE_RGB_LED_H
#define REGISTRY_SYS_NAMESPACE_RGB_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry/sys_namespace/util.h"

int _registry_sys_namespace_util_get_buf(const registry_schema_t *schema,
                                         const registry_instance_t *instance,
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

int _registry_sys_namespace_util_set_buf(const registry_schema_t *schema,
                                         const registry_instance_t *instance,
                                         const registry_schema_item_t *parameter,
                                         const registry_type_t type,
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

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_SYS_NAMESPACE_RGB_LED_H */
/** @} */
