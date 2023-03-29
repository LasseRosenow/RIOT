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
#include "debug.h"
#include "base64.h"
#include "kernel_defines.h"

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
        case REGISTRY_TYPE_GROUP: break;
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

const registry_namespace_t *registry_util_namespace_lookup(const registry_id_t namespace_id)
{
    clist_node_t *node = _registry_namespaces.next;

    if (!node) {
        return NULL;
    }

    registry_id_t index = 0;

    do {
        node = node->next;
        const registry_namespace_t *namespace = container_of(node, registry_namespace_t, node);

        if (index == namespace_id) {
            return namespace;
        }

        index++;
    } while (node != _registry_namespaces.next);

    return NULL;
}

const registry_schema_t *registry_util_schema_lookup(const registry_namespace_t *namespace,
                                                     const registry_id_t schema_id)
{
    for (size_t i = 0; i < namespace->items_len; i++) {
        const registry_schema_t *schema = namespace->items[i];

        if (schema->id == schema_id) {
            return schema;
        }
    }

    return NULL;
}

const registry_instance_t *registry_util_instance_lookup(const registry_schema_t *schema,
                                                         const registry_id_t instance_id)
{
    assert(schema != NULL);

    /* find instance with correct instance_id */
    clist_node_t *node = schema->instances.next;

    if (!node) {
        return NULL;
    }

    registry_id_t index = 0;

    do {
        node = node->next;
        const registry_instance_t *instance = container_of(node, registry_instance_t, node);

        /* check if index equals instance_id */
        if (index == instance_id) {
            return instance;
        }

        index++;
    } while (node != schema->instances.next);

    return NULL;
}

int registry_util_convert_str_to_value(const char *src, void *dest, const size_t dest_len,
                                       const registry_type_t dest_type)
{
    assert(src != NULL);
    assert(dest != NULL);

    char *eptr = '\0';

    if (!src) {
        return -EINVAL;
    }

    switch (dest_type) {
    case REGISTRY_TYPE_NONE:
    case REGISTRY_TYPE_GROUP: {
        return -EINVAL;
    }

    case REGISTRY_TYPE_OPAQUE: {
        size_t base64_decode_len;
        if (base64_decode(src, strlen(src), dest, &base64_decode_len) !=
            BASE64_SUCCESS && base64_decode_len <= dest_len) {
            return -EINVAL;
        }
        break;
    }

    case REGISTRY_TYPE_STRING: {
        if (strlen(src) + 1 > dest_len) {
            return -EINVAL;
        }
        strcpy((char *)dest, src);
        break;
    }

    case REGISTRY_TYPE_BOOL: {
        *(bool *)dest = strtol(src, &eptr, 0);
        break;
    }

    case REGISTRY_TYPE_UINT8: {
        *(uint8_t *)dest = strtoul(src, &eptr, 0);
        break;
    }

    case REGISTRY_TYPE_UINT16: {
        *(uint16_t *)dest = strtoul(src, &eptr, 0);
        break;
    }

    case REGISTRY_TYPE_UINT32: {
        *(uint32_t *)dest = strtoul(src, &eptr, 0);
        break;
    }

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
    case REGISTRY_TYPE_UINT64: {
        *(uint64_t *)dest = strtoull(src, &eptr, 0);
        break;
    }
#endif /* CONFIG_REGISTRY_USE_UINT64 */

    case REGISTRY_TYPE_INT8: {
        *(int8_t *)dest = strtol(src, &eptr, 0);
        break;
    }

    case REGISTRY_TYPE_INT16: {
        *(int16_t *)dest = strtol(src, &eptr, 0);
        break;
    }

    case REGISTRY_TYPE_INT32: {
        *(int32_t *)dest = strtol(src, &eptr, 0);
        break;
    }

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
    case REGISTRY_TYPE_INT64: {
        *(int64_t *)dest = strtoll(src, &eptr, 0);
        break;
    }
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32)
    case REGISTRY_TYPE_FLOAT32: {
        *(float *)dest = strtof(src, &eptr);
        break;
    }
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
    case REGISTRY_TYPE_FLOAT64: {
        *(double *)dest = strtod(src, &eptr);
        break;
    }
#endif /* CONFIG_REGISTRY_USE_FLOAT64 */
    }

    if (*eptr != '\0') {
        return -EINVAL;
    }

    return 0;
}

int registry_util_convert_value_to_str(const registry_value_t *src, char *dest,
                                       const size_t dest_len)
{
    assert(src != NULL);

    size_t str_len;

    switch (src->type) {
    case REGISTRY_TYPE_NONE:
    case REGISTRY_TYPE_GROUP: {
        return -EINVAL;
    }

    case REGISTRY_TYPE_OPAQUE: {
        if (base64_encode(src->buf, src->buf_len, dest, &str_len) != BASE64_SUCCESS
            || str_len > dest_len - 1) {
            /* If dest is NULL, the length is returned */
            if (dest != NULL) {
                return -EINVAL;
            }
        }
        else {
            dest[str_len] = '\0';
        }
        break;
    }

    case REGISTRY_TYPE_STRING: {
        char *str_val = (char *)src->buf;

        str_len = strlen(str_val);

        if (str_len > dest_len - 1) {
            /* If dest is NULL, the length is returned */
            if (dest != NULL) {
                return -EINVAL;
            }
        }
        else {
            strcpy(dest, str_val);
        }
        break;
    }

    case REGISTRY_TYPE_BOOL: {
        str_len = snprintf(dest, dest_len, " %" PRId8, *(bool *)src->buf);
        break;
    }

    case REGISTRY_TYPE_UINT8: {
        str_len = snprintf(dest, dest_len, " %" PRIu8, *(uint8_t *)src->buf);
        break;
    }

    case REGISTRY_TYPE_UINT16: {
        str_len = snprintf(dest, dest_len, " %" PRIu16, *(uint16_t *)src->buf);
        break;
    }

    case REGISTRY_TYPE_UINT32: {
        str_len = snprintf(dest, dest_len, " %" PRIu32, *(uint32_t *)src->buf);
        break;
    }

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
    case REGISTRY_TYPE_UINT64: {
        str_len = fmt_u64_dec(NULL, *(uint64_t *)src->buf);
        if (str_len > dest_len - 1) {
            /* If dest is NULL, the length is returned */
            if (dest != NULL) {
                return -EINVAL;
            }
        }
        else {
            fmt_u64_dec(dest, *(uint64_t *)src->buf);
            dest[str_len] = '\0';
        }
        break;
    }
#endif /* CONFIG_REGISTRY_USE_UINT64 */

    case REGISTRY_TYPE_INT8: {
        str_len = snprintf(dest, dest_len, " %" PRId8, *(int8_t *)src->buf);
        break;
    }

    case REGISTRY_TYPE_INT16: {
        str_len = snprintf(dest, dest_len, " %" PRId16, *(int16_t *)src->buf);
        break;
    }

    case REGISTRY_TYPE_INT32: {
        str_len = snprintf(dest, dest_len, " %" PRId32, *(int32_t *)src->buf);
        break;
    }

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
    case REGISTRY_TYPE_INT64: {
        str_len = fmt_s64_dec(NULL, *(int64_t *)src->buf);
        if (str_len > dest_len - 1) {
            /* If dest is NULL, the length is returned */
            if (dest != NULL) {
                return -EINVAL;
            }
        }
        else {
            fmt_s64_dec(dest, *(int64_t *)src->buf);
            dest[str_len] = '\0';
        }
        break;
    }
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32)
    case REGISTRY_TYPE_FLOAT32: {
        str_len = sprintf(dest, " %f", *(float *)src->buf);
        if (str_len > dest_len - 1) {
            /* If dest is NULL, the length is returned */
            if (dest != NULL) {
                return -EINVAL;
            }
        }
        break;
    }
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
    case REGISTRY_TYPE_FLOAT64: {
        str_len = sprintf(dest, " %f", *(double *)src->buf);
        if (str_len > dest_len - 1) {
            /* If dest is NULL, the length is returned */
            if (dest != NULL) {
                return -EINVAL;
            }
        }
        break;
    }
#endif /* CONFIG_REGISTRY_USE_FLOAT64 */
    }

    return str_len;
}
