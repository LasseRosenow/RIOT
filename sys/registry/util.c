/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_util RIOT Registry utilities
 * @ingroup     sys
 * @brief       RIOT Registry Util module providing utility functions
 * @{
 *
 * @file
 *
 * @author      Leandro Lanzieri <leandro.lanzieri@haw-hamburg.de>
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <assert.h>
#define ENABLE_DEBUG (0)
#include <debug.h>
#include <base64.h>
#include <kernel_defines.h>

#include "registry.h"
#include "registry/util.h"

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64) || defined(CONFIG_REGISTRY_USE_UINT64)
#include <fmt.h>
#endif /* CONFIG_REGISTRY_USE_INT64 || CONFIG_REGISTRY_USE_UINT64 */

void _debug_print_value(const registry_value_t *value)
{
    if (ENABLE_DEBUG) {
        switch (value->type) {
        case REGISTRY_TYPE_NONE: break;
        case REGISTRY_TYPE_OPAQUE: {
            DEBUG("opaque (hex): ");
            for (size_t i = 0; i < value->buf_len; i++) {
                DEBUG("%02x", ((uint8_t *)value->buf)[i]);
            }
            break;
        }
        case REGISTRY_TYPE_STRING: DEBUG("string: %s", (char *)value->buf); break;
        case REGISTRY_TYPE_BOOL: DEBUG("bool: %d", *(bool *)value->buf); break;

        case REGISTRY_TYPE_UINT8: DEBUG("uint8: %d", *(uint8_t *)value->buf); break;
        case REGISTRY_TYPE_UINT16: DEBUG("uint16: %d", *(uint16_t *)value->buf); break;
        case REGISTRY_TYPE_UINT32: DEBUG("uint32: %d", *(uint32_t *)value->buf); break;
    #if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
        case REGISTRY_TYPE_UINT64: DEBUG("uint64: %lld", *(uint64_t *)value->buf); break;
    #endif /* CONFIG_REGISTRY_USE_UINT64 */

        case REGISTRY_TYPE_INT8: DEBUG("int8: %d", *(int8_t *)value->buf); break;
        case REGISTRY_TYPE_INT16: DEBUG("int16: %d", *(int16_t *)value->buf); break;
        case REGISTRY_TYPE_INT32: DEBUG("int32: %d", *(int32_t *)value->buf); break;

    #if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
        case REGISTRY_TYPE_INT64: DEBUG("int64: %lld", *(int64_t *)value->buf); break;
    #endif /* CONFIG_REGISTRY_USE_INT64 */

    #if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32)
        case REGISTRY_TYPE_FLOAT32: DEBUG("f32: %f", *(float *)value->buf); break;
    #endif /* CONFIG_REGISTRY_USE_FLOAT32 */

    #if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
        case REGISTRY_TYPE_FLOAT64: DEBUG("f64: %f", *(double *)value->buf); break;
    #endif /* CONFIG_REGISTRY_USE_FLOAT32 */
        }
    }
}

registry_namespace_t *_namespace_lookup(const registry_namespace_id_t namespace_id)
{
    switch (namespace_id) {
    case REGISTRY_NAMESPACE_SYS:
        return &registry_namespace_sys;
    case REGISTRY_NAMESPACE_APP:
        return &registry_namespace_app;
    }

    return NULL;
}

static int _registry_cmp_schema_id(clist_node_t *current, void *id)
{
    assert(current != NULL);
    registry_schema_t *schema = container_of(current, registry_schema_t, node);

    return !(schema->id - *(int *)id);
}

registry_schema_t *_schema_lookup(const registry_namespace_t *namespace,
                                  const int schema_id)
{
    clist_node_t *node;
    registry_schema_t *schema = NULL;

    node =
        clist_foreach((clist_node_t *)&namespace->schemas, _registry_cmp_schema_id,
                      (clist_node_t *)&schema_id);

    if (node != NULL) {
        schema = container_of(node, registry_schema_t, node);
    }

    return schema;
}

registry_instance_t *_instance_lookup(const registry_schema_t *schema, const int instance_id)
{
    assert(schema != NULL);

    /* find instance with correct instance_id */
    clist_node_t *node = schema->instances.next;
    int index = 0;

    do {
        node = node->next;
        registry_instance_t *instance = container_of(node, registry_instance_t, node);

        /* check if index equals instance_id */
        if (index == instance_id) {
            return instance;
        }

        index++;
    } while (node != schema->instances.next);

    return NULL;
}

static int _get_string_len(const registry_value_t *value)
{
    switch (value->type) {
    case REGISTRY_TYPE_NONE: return -EINVAL;
    case REGISTRY_TYPE_OPAQUE: return -EINVAL;
    case REGISTRY_TYPE_STRING: return strlen((char *)value->buf);
    case REGISTRY_TYPE_BOOL: return snprintf(NULL, 0, "%d", *(bool *)value->buf);

    case REGISTRY_TYPE_UINT8: return snprintf(NULL, 0, "%d", *(uint8_t *)value->buf);
    case REGISTRY_TYPE_UINT16: return snprintf(NULL, 0, "%d", *(uint16_t *)value->buf);
    case REGISTRY_TYPE_UINT32: return snprintf(NULL, 0, "%d", *(uint32_t *)value->buf);
#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
    case REGISTRY_TYPE_UINT64: return snprintf(NULL, 0, "%lld", *(uint64_t *)value->buf);
#endif /* CONFIG_REGISTRY_USE_UINT64 */

    case REGISTRY_TYPE_INT8: return snprintf(NULL, 0, "%d", *(int8_t *)value->buf);
    case REGISTRY_TYPE_INT16: return snprintf(NULL, 0, "%d", *(int16_t *)value->buf);
    case REGISTRY_TYPE_INT32: return snprintf(NULL, 0, "%d", *(int32_t *)value->buf);

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
    case REGISTRY_TYPE_INT64: return snprintf(NULL, 0, "%lld", *(int64_t *)value->buf);
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32)
    case REGISTRY_TYPE_FLOAT32: return snprintf(NULL, 0, "%f", *(float *)value->buf);
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
    case REGISTRY_TYPE_FLOAT64: return snprintf(NULL, 0, "%f", *(double *)value->buf);
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */
    }

    return 0;
}



int registry_convert_str_to_value(const char *src, void *dest, const size_t dest_len,
                                  const registry_type_t dest_type)
{
    assert(src != NULL);

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
    int64_t val_i = 0;

#else /* CONFIG_REGISTRY_USE_INT64 */
    int32_t val_i = 0;
#endif

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
    uint64_t val_u = 0;
#else /* CONFIG_REGISTRY_USE_UINT64 */
    uint32_t val_u = 0;
#endif

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
    double val_f;
#elif IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32)
    float val_f;
#endif /* CONFIG_REGISTRY_USE_FLOAT64 || CONFIG_REGISTRY_USE_FLOAT32 */

    char *eptr = 0;

    if (!src) {
        goto err;
    }

    switch (dest_type) {
    case REGISTRY_TYPE_STRING:
        if (strlen(src) + 1 > dest_len) {
            goto err;
        }
        strcpy((char *)dest, src);
        break;

    case REGISTRY_TYPE_UINT8:
    case REGISTRY_TYPE_UINT16:
    case REGISTRY_TYPE_UINT32:
        val_u = strtoul(src, &eptr, 0);
        if (*eptr != '\0') {
            goto err;
        }
        else if (dest_type == REGISTRY_TYPE_UINT8) {
            if (val_u > UINT8_MAX) {
                goto err;
            }
            *(uint8_t *)dest = val_u;
        }
        else if (dest_type == REGISTRY_TYPE_UINT16) {
            if (val_u > UINT16_MAX) {
                goto err;
            }
            *(uint16_t *)dest = val_u;
        }
        else if (dest_type == REGISTRY_TYPE_UINT32) {
            *(uint32_t *)dest = val_u;
        }
        break;

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
    case REGISTRY_TYPE_UINT64:
        val_u = strtoull(src, &eptr, 0);
        if (*eptr != '\0') {
            goto err;
        }
        *(uint64_t *)dest = val_u;
        break;
#endif /* CONFIG_REGISTRY_USE_UINT64 */

    case REGISTRY_TYPE_INT8:
    case REGISTRY_TYPE_INT16:
    case REGISTRY_TYPE_INT32:
    case REGISTRY_TYPE_BOOL:
        val_i = strtol(src, &eptr, 0);
        if (*eptr != '\0') {
            goto err;
        }
        if (dest_type == REGISTRY_TYPE_BOOL) {
            if (val_i < 0 || val_i > 1) {
                goto err;
            }
            *(bool *)dest = val_i;
        }
        else if (dest_type == REGISTRY_TYPE_INT8) {
            if (val_i < INT8_MIN || val_i > INT8_MAX) {
                goto err;
            }
            *(int8_t *)dest = val_i;
        }
        else if (dest_type == REGISTRY_TYPE_INT16) {
            if (val_i < INT16_MIN || val_i > INT16_MAX) {
                goto err;
            }
            *(int16_t *)dest = val_i;
        }
        else if (dest_type == REGISTRY_TYPE_INT32) {
            *(int32_t *)dest = val_i;
        }
        break;

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
    case REGISTRY_TYPE_INT64:
        val_i = strtoull(src, &eptr, 0);
        if (*eptr != '\0') {
            goto err;
        }
        *(int64_t *)dest = val_i;
        break;
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32)
    case REGISTRY_TYPE_FLOAT32:
        val_f = strtof(src, &eptr);
        if (*eptr != '\0') {
            goto err;
        }
        *(float *)dest = val_f;
        break;
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
    case REGISTRY_TYPE_FLOAT64:
        val_f = strtod(src, &eptr);
        if (*eptr != '\0') {
            goto err;
        }
        *(double *)dest = val_f;
        break;
#endif /* CONFIG_REGISTRY_USE_FLOAT64 */
    default:
        goto err;
    }
    return 0;
err:
    return -EINVAL;
}

int registry_convert_value_to_value(const registry_value_t *src, void *dest,
                                    const size_t dest_len, const registry_type_t dest_type)
{
    char string[_get_string_len(src)];

    char *new_string =
        registry_convert_value_to_str(src, string, ARRAY_SIZE(string));

    if (new_string == NULL) {
        return -EINVAL;
    }

    int new_value_success = registry_convert_str_to_value(string, dest, dest_len, dest_type);

    return new_value_success;
}

int registry_convert_str_to_bytes(const char *src, void *dest, size_t *dest_len)
{
    assert(dest != NULL);
    size_t val_len = strlen(src);

    if (base64_decode((unsigned char *)src, val_len, (void *)dest, dest_len) !=
        BASE64_SUCCESS) {
        return -EINVAL;
    }

    return 0;
}

char *registry_convert_value_to_str(const registry_value_t *src, char *dest,
                                    const size_t dest_len)
{
    assert(src != NULL);

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
    int64_t val_u64 = 0;
#endif /* CONFIG_REGISTRY_USE_UINT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
    int64_t val_i64 = 0;
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64) || IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32) || \
    IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
    size_t len;
#endif /* CONFIG_REGISTRY_USE_INT64 || CONFIG_REGISTRY_USE_FLOAT32 || CONFIG_REGISTRY_USE_FLOAT64 */

    uint32_t val_u = 0;
    int32_t val_i = 0;

    switch (src->type) {
    case REGISTRY_TYPE_STRING: {
        char *str_val = (char *)src->buf;

        if (strlen(str_val) <= dest_len) {
            strcpy(dest, str_val);
            return dest;
        }
        else {
            return NULL;
        }
    }

    case REGISTRY_TYPE_UINT8:
    case REGISTRY_TYPE_UINT16:
    case REGISTRY_TYPE_UINT32:
        if (src->type == REGISTRY_TYPE_UINT8) {
            val_u = *(uint8_t *)src->buf;
        }
        else if (src->type == REGISTRY_TYPE_UINT16) {
            val_u = *(uint16_t *)src->buf;
        }
        else if (src->type == REGISTRY_TYPE_UINT32) {
            val_u = *(uint32_t *)src->buf;
        }
        snprintf(dest, dest_len, " %" PRIu32, val_u);
        return dest;

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
    case REGISTRY_TYPE_UINT64:
        val_u64 = *(uint64_t *)src->buf;
        len = fmt_u64_dec(NULL, val_u64);
        if (len > dest_len - 1) {
            return NULL;
        }
        fmt_u64_dec(dest, val_u64);
        dest[len] = '\0';
        return dest;
#endif /* CONFIG_REGISTRY_USE_INT64 */

    case REGISTRY_TYPE_INT8:
    case REGISTRY_TYPE_INT16:
    case REGISTRY_TYPE_INT32:
    case REGISTRY_TYPE_BOOL:
        if (src->type == REGISTRY_TYPE_INT8) {
            val_i = *(int8_t *)src->buf;
        }
        else if (src->type == REGISTRY_TYPE_INT16) {
            val_i = *(int16_t *)src->buf;
        }
        else if (src->type == REGISTRY_TYPE_INT32) {
            val_i = *(int32_t *)src->buf;
        }
        else if (src->type == REGISTRY_TYPE_BOOL) {
            val_i = *(bool *)src->buf;
        }
        snprintf(dest, dest_len, " %" PRId32, val_i);
        return dest;

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
    case REGISTRY_TYPE_INT64:
        val_i64 = *(int64_t *)src->buf;
        len = fmt_s64_dec(NULL, val_i64);
        if (len > dest_len - 1) {
            return NULL;
        }
        fmt_s64_dec(dest, val_i64);
        dest[len] = '\0';
        return dest;
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32)
    case REGISTRY_TYPE_FLOAT32:
        sprintf(dest, " %f", *(float *)src->buf);
        len = strlen(dest);
        if (len > dest_len - 1) {
            return NULL;
        }
        return dest;
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
    case REGISTRY_TYPE_FLOAT64:
        sprintf(dest, " %f", *(double *)src->buf);
        len = strlen(dest);
        if (len > dest_len - 1) {
            return NULL;
        }
        return dest;
#endif /* CONFIG_REGISTRY_USE_FLOAT64 */

    default:
        return NULL;
    }

    return dest;
}

char *registry_convert_bytes_to_str(const void *src, const size_t src_len, char *dest,
                                    size_t *dest_len)
{
    assert(src != NULL);

    base64_encode(src, src_len, dest, dest_len);

    dest[*dest_len] = '\0';

    return dest;
}
