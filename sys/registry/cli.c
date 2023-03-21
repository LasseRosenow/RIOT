/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_cli RIOT Registry CLI
 * @ingroup     sys
 * @brief       RIOT Registry module providing a CLI for the RIOT Registry sys module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "registry.h"
#include "registry/util.h"
#include "registry/path.h"
#include "registry/path/util.h"

#include "registry/cli.h"

/**
 * @brief Separator character to define hierarchy in configurations names.
 */
#define REGISTRY_CLI_PATH_SEPARATOR    '/'

static void _print_registry_value(const registry_value_t *value)
{
    switch (value->type) {
    case REGISTRY_TYPE_NONE: break;
    case REGISTRY_TYPE_GROUP: break;
    case REGISTRY_TYPE_OPAQUE: {
        printf("opaque (hex): ");
        for (size_t i = 0; i < value->buf_len; i++) {
            printf("%02x", ((uint8_t *)value->buf)[i]);
        }
        break;
    }
    case REGISTRY_TYPE_STRING: printf("string: %s", (char *)value->buf); break;
    case REGISTRY_TYPE_BOOL: printf("bool: %d", *(bool *)value->buf); break;

    case REGISTRY_TYPE_UINT8: printf("uint8: %d", *(uint8_t *)value->buf); break;
    case REGISTRY_TYPE_UINT16: printf("uint16: %d", *(uint16_t *)value->buf); break;
    case REGISTRY_TYPE_UINT32: printf("uint32: %d", *(uint32_t *)value->buf); break;
#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64)
    case REGISTRY_TYPE_UINT64: printf("uint64: %lld", *(uint64_t *)value->buf); break;
#endif /* CONFIG_REGISTRY_USE_UINT64 */

    case REGISTRY_TYPE_INT8: printf("int8: %d", *(int8_t *)value->buf); break;
    case REGISTRY_TYPE_INT16: printf("int16: %d", *(int16_t *)value->buf); break;
    case REGISTRY_TYPE_INT32: printf("int32: %d", *(int32_t *)value->buf); break;

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64)
    case REGISTRY_TYPE_INT64: printf("int64: %lld", *(int64_t *)value->buf); break;
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32)
    case REGISTRY_TYPE_FLOAT32: printf("f32: %f", *(float *)value->buf); break;
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64)
    case REGISTRY_TYPE_FLOAT64: printf("f64: %f", *(double *)value->buf); break;
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */
    }
}

static int _export_cb(const registry_path_t *path,
                      const registry_export_cb_data_t *data,
                      const registry_export_cb_data_type_t data_type,
                      const registry_value_t *value,
                      const void *context)
{
    (void)value;
    (void)context;

    /* calculate the indentation based on the the exported data type */
    /* fallthrough switch is intentional */
    /* the more nesting we have, the more indentation we need. => highest nesting level first */
    size_t path_len = path->path_len;

    switch (data_type) {
    case REGISTRY_EXPORT_PARAMETER:
        printf("%d %s\n", data->parameter->id, data->parameter->name);
        __attribute__ ((fallthrough));

    case REGISTRY_EXPORT_GROUP:
        printf("%d %s\n", data->group->id, data->group->name);
        __attribute__ ((fallthrough));

    case REGISTRY_EXPORT_INSTANCE:
        path_len++;
        __attribute__ ((fallthrough));

    case REGISTRY_EXPORT_SCHEMA:
        path_len++;
        __attribute__ ((fallthrough));

    case REGISTRY_EXPORT_NAMESPACE:
        path_len++;
    }

    printf("%*c\b", ((path_len - 1) * 2) + 1, ' ');

    /* print the path element, that is currently being exported */
    switch (data_type) {
    case REGISTRY_EXPORT_NAMESPACE:
        printf("%d %s\n", *path->namespace_id, data->namespace->name);
        break;

    case REGISTRY_EXPORT_SCHEMA:
        printf("%d %s\n", *path->schema_id, data->schema->name);
        break;

    case REGISTRY_EXPORT_INSTANCE:
        printf("%d %s\n", *path->instance_id, data->instance->name);
        break;

    case REGISTRY_EXPORT_GROUP:
        printf("%d %s\n", data->group->id, data->group->name);
        break;

    case REGISTRY_EXPORT_PARAMETER:
        printf("%d %s\n", data->parameter->id, data->parameter->name);
        break;
    }

    return 0;
}

int registry_cli_cmd(int argc, char **argv)
{
    registry_path_t path = REGISTRY_PATH();
    registry_id_t path_items[REGISTRY_PATH_ITEMS_MAX_LEN];

    if (argc == 1) {
        /* show help for main commands */
        goto help_error;
    }

    if (strcmp(argv[1], "get") == 0) {
        if (registry_path_util_parse_string_path(argv[2], &path, path_items) < 0) {
            printf("usage: %s %s <path>\n", argv[0], argv[1]);
            return 1;
        }

        registry_value_t value;
        int res = registry_get_by_path(&path, &value);

        if (res != 0) {
            printf("error: %d\n", res);
            return 1;
        }

        _print_registry_value(&value);
        printf("\n");

        return 0;
    }
    else if (strcmp(argv[1], "set") == 0) {
        if (registry_path_util_parse_string_path(argv[2], &path, path_items) < 0) {
            printf("usage: %s %s <path> <value>\n", argv[0], argv[1]);
            return 1;
        }

        // TODO registry_set_string_by_path(path, argv[3]);
        return 0;
    }
    else if (strcmp(argv[1], "commit") == 0) {
        if (registry_path_util_parse_string_path(argv[2], &path, path_items) < 0) {
            printf("usage: %s %s <path>\n", argv[0], argv[1]);
            return 1;
        }

        registry_commit_by_path(&path);
        return 0;
    }
    else if (strcmp(argv[1], "export") == 0) {
        /* If the path is invalid, it can also just be non existent, so other arguments like -r need to be checked */
        bool invalid_path = false;
        if (registry_path_util_parse_string_path(argv[2], &path, path_items) < 0) {
            invalid_path = true;
        }
        if (invalid_path && strcmp(argv[2], "-r") != 0) {
            printf("usage: %s %s <path> [-r <recursion depth>]\n", argv[0], argv[1]);
            return 1;
        }

        /* The argv index of -r varies depending on if a path was specified or not */
        int recursion_level = 0;
        if (invalid_path && argc > 3 && strcmp(argv[2], "-r") == 0) {
            recursion_level = atoi(argv[3]);
        }
        else if (argc > 4 && strcmp(argv[3], "-r") == 0) {
            recursion_level = atoi(argv[4]);
        }

        registry_export_by_path(_export_cb, &path, recursion_level, NULL);
        return 0;
    }
    else if (strcmp(argv[1], "load") == 0) {
        if (argc > 2) {
            if (registry_path_util_parse_string_path(argv[2], &path, path_items) < 0) {
                printf("usage: %s %s [path]\n", argv[0], argv[1]);
                return 1;
            }
            else {
                registry_load_by_path(&path);
            }
        }
        else {
            registry_path_t new_path = REGISTRY_PATH();
            registry_load_by_path(&new_path);
        }

        return 0;
    }
    else if (strcmp(argv[1], "save") == 0) {
        if (argc > 2) {
            if (registry_path_util_parse_string_path(argv[2], &path, path_items) < 0) {
                printf("usage: %s %s [path]\n", argv[0], argv[1]);
                return 1;
            }
            else {
                registry_save_by_path(&path);
            }
        }
        else {
            registry_path_t new_path = REGISTRY_PATH();
            registry_save_by_path(&new_path);
        }

        return 0;
    }

help_error:
    printf("usage: %s {get|set|commit|export|load|save}\n", argv[0]);

    return 1;
}
