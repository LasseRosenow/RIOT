/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/**
 * @ingroup     sys_shell_commands
 * @{
 *
 * @file
 * @brief       Registry shell commands
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
#include "registry/int_path.h"
#include "registry/storage.h"
#include "shell.h"

/**
 * @brief Separator character to define hierarchy in configurations names.
 */
#define REGISTRY_CLI_PATH_SEPARATOR    '/'

static int _parse_string_path(const char *string_path,
                              registry_int_path_t *registry_path,
                              registry_int_path_type_t *registry_path_type)
{
    char *ptr = (char *)string_path;

    if (*ptr == '\0') {
        return -EINVAL;
    }

    /* get namespace_id */
    registry_namespace_id_t namespace_id = strtol(ptr, &ptr, 10);

    if (*ptr == '\0') {
        registry_path->namespace_path = (registry_namespace_int_path_t) {
            .namespace_id = namespace_id,
        };
        *registry_path_type = REGISTRY_INT_PATH_TYPE_NAMESPACE;
        return 0;
    }

    /* get schema_id */
    registry_schema_id_t schema_id = strtol(ptr, &ptr, 10);

    if (*ptr == '\0') {
        registry_path->schema_path = (registry_schema_int_path_t) {
            .namespace_id = namespace_id,
            .schema_id = schema_id,
        };
        *registry_path_type = REGISTRY_INT_PATH_TYPE_SCHEMA;
        return 0;
    }

    /* get instance_id */
    registry_instance_id_t instance_id = strtol(ptr, &ptr, 10);

    if (*ptr == '\0') {
        registry_path->instance_path = (registry_instance_int_path_t) {
            .namespace_id = namespace_id,
            .schema_id = schema_id,
            .instance_id = instance_id,
        };
        *registry_path_type = REGISTRY_INT_PATH_TYPE_INSTANCE;
        return 0;
    }

    /* get resource_id */
    // TODO FIX THIS
    // registry_resource_id_t resource_id = strtol(ptr, &ptr, 10);

    // if (*ptr == '\0') {
    //     registry_path->resource_path = (registry_resource_int_path_t) {
    //         .namespace_id = namespace_id,
    //         .schema_id = schema_id,
    //         .instance_id = instance_id,
    //         .resource_id = resource_id,
    //     };
    //     *registry_path_type = REGISTRY_INT_PATH_TYPE_RESOURCE;
    //     return 0;
    // }

    return 0;
}

// static int _export_cb(const registry_resource_path_t *path,
//                       const registry_export_cb_data_t *data,
//                       const registry_export_cb_data_type_t data_type,
//                       const registry_value_t *value,
//                       const void *context)
// {
//     (void)value;
//     (void)context;

//     /* calculate the indentation based on the the exported data type */
//     /* fallthrough switch is intentional */
//     /* the more nesting we have, the more indentation we need. => highest nesting level first */
//     size_t path_len = 0;

//     switch (data_type) {
//     case REGISTRY_EXPORT_PARAMETER:
//         printf("%d %s\n", data->parameter->id, data->parameter->name);
//         __attribute__ ((fallthrough));

//     case REGISTRY_EXPORT_GROUP:
//         printf("%d %s\n", data->group->id, data->group->name);
//         __attribute__ ((fallthrough));

//     case REGISTRY_EXPORT_INSTANCE:
//         path_len++;
//         __attribute__ ((fallthrough));

//     case REGISTRY_EXPORT_SCHEMA:
//         path_len++;
//         __attribute__ ((fallthrough));

//     case REGISTRY_EXPORT_NAMESPACE:
//         path_len++;
//     }

//     printf("%*c\b", ((path_len - 1) * 2) + 1, ' ');

//     /* print the path element, that is currently being exported */
//     switch (data_type) {
//     case REGISTRY_EXPORT_NAMESPACE:
//         printf("%d %s\n", *path->namespace_id, data->namespace->name);
//         break;

//     case REGISTRY_EXPORT_SCHEMA:
//         printf("%d %s\n", *path->schema_id, data->schema->name);
//         break;

//     case REGISTRY_EXPORT_INSTANCE:
//         printf("%d %s\n", *path->instance_id, data->instance->name);
//         break;

//     case REGISTRY_EXPORT_GROUP:
//         printf("%d %s\n", data->group->id, data->group->name);
//         break;

//     case REGISTRY_EXPORT_PARAMETER:
//         printf("%d %s\n", data->parameter->id, data->parameter->name);
//         break;
//     }

//     return 0;
// }

static int _registry(int argc, char **argv)
{
    registry_int_path_t path;
    registry_int_path_type_t path_type;
    registry_namespace_t *namespace;
    registry_schema_t *schema;
    registry_instance_t *instance;
    registry_group_t *group;
    registry_parameter_t *parameter;
    registry_value_t value;

    if (argc == 1) {
        /* show help for main commands */
        goto help_error;
    }

    if (strcmp(argv[1], "get") == 0) {
        if (_parse_string_path(argv[2], &path, &path_type) < 0) {
            printf("usage: %s %s <path>\n", argv[0], argv[1]);
            return 1;
        }

        if (path_type != REGISTRY_INT_PATH_TYPE_PARAMETER) {
            return -EINVAL;
        }

        /* get instance and parameter of the path */
        int res =
            registry_from_parameter_int_path(&path.parameter_path, NULL, NULL, &instance,
                                             &parameter);
        if (res == 0) {
            res = registry_get(instance, parameter, &value);

            if (res == 0) {
                /* get the length of the value as a string */
                size_t str_len = registry_util_convert_value_to_str(&value, NULL, 0);

                /* convert the value to a string */
                char str[str_len + 1];
                registry_util_convert_value_to_str(&value, str, str_len + 1);

                /* print the string */
                printf("%s\n", str);
                return 0;
            }
        }

        printf("error: %d\n", res);
        return 1;
    }
    else if (strcmp(argv[1], "set") == 0) {
        if (_parse_string_path(argv[2], &path, &path_type) < 0) {
            printf("usage: %s %s <path> <value>\n", argv[0], argv[1]);
            return 1;
        }

        if (path_type != REGISTRY_INT_PATH_TYPE_PARAMETER) {
            return -EINVAL;
        }

        /* get instance and parameter of the path */
        int res =
            registry_from_parameter_int_path(&path.parameter_path, NULL, NULL, &instance,
                                             &parameter);
        if (res == 0) {
            /* get value from the registry, to know its correct type and size */
            res = registry_get(instance, parameter, &value);

            if (res == 0) {
                /* convert the string into the correct value type */
                uint8_t new_value_buf[value.buf_len];
                registry_util_convert_str_to_value(argv[3], new_value_buf, value.buf_len,
                                                   value.type);

                /* let the registry_value_t object point to the buffer of the new value */
                value.buf = new_value_buf;

                /* set the new value in the registry */
                registry_set(instance, parameter, value.buf, value.buf_len);
                return 0;
            }
        }

        printf("error: %d\n", res);
        return 1;
    }
    else if (strcmp(argv[1], "commit") == 0) {
        if (_parse_string_path(argv[2], &path, &path_type) < 0) {
            printf("usage: %s %s <path>\n", argv[0], argv[1]);
            return 1;
        }

        int res = 0;

        /* commit depending on the path type */
        if (res == 0) {
            switch (path_type) {
            case REGISTRY_INT_PATH_TYPE_NAMESPACE:
                registry_from_namespace_int_path(&path.namespace_path, &namespace);
                registry_commit_namespace(namespace);
                break;

            case REGISTRY_INT_PATH_TYPE_SCHEMA:
                registry_from_schema_int_path(&path.schema_path, &namespace, &schema);
                registry_commit_schema(schema);
                break;

            case REGISTRY_INT_PATH_TYPE_INSTANCE:
                registry_from_instance_int_path(&path.instance_path, &namespace, &schema,
                                                &instance);
                registry_commit_instance(instance);
                break;

            case REGISTRY_INT_PATH_TYPE_GROUP:
                registry_from_group_int_path(&path.group_path, &namespace, &schema, &instance,
                                             &group);
                registry_commit_group(instance, group);
                break;

            case REGISTRY_INT_PATH_TYPE_PARAMETER:
                registry_from_parameter_int_path(&path.parameter_path, &namespace, &schema,
                                                 &instance, &parameter);
                registry_commit_parameter(instance, parameter);
                break;
            }
            return 0;
        }

        printf("error: %d\n", res);
        return 1;
    }
    else if (strcmp(argv[1], "export") == 0) {
        // TODO export is now very different because of new path API

        // /* if the path is invalid, it can also just be non existent, so other arguments like -r need to be checked */
        // bool invalid_path = false;
        // if (_parse_string_path(argv[2], &path) < 0) {
        //     invalid_path = true;
        // }
        // if (invalid_path && strcmp(argv[2], "-r") != 0) {
        //     printf("usage: %s %s <path> [-r <recursion depth>]\n", argv[0], argv[1]);
        //     return 1;
        // }

        // /* the argv index of -r varies depending on if a path was specified or not */
        // int recursion_level = 0;
        // if (invalid_path && argc > 3 && strcmp(argv[2], "-r") == 0) {
        //     recursion_level = atoi(argv[3]);
        // }
        // else if (argc > 4 && strcmp(argv[3], "-r") == 0) {
        //     recursion_level = atoi(argv[4]);
        // }

        // registry_export_by_path(_export_cb, &path, recursion_level, NULL);
        return 0;
    }
#if IS_USED(MODULE_REGISTRY_STORAGE) || IS_ACTIVE(DOXYGEN)
    // TODO

    // else if (strcmp(argv[1], "load") == 0) {
    //     if (argc > 2) {
    //         if (_parse_string_path(argv[2], &path) < 0) {
    //             printf("usage: %s %s [path]\n", argv[0], argv[1]);
    //             return 1;
    //         }
    //         else {
    //             registry_load_by_path(&path);
    //         }
    //     }
    //     else {
    //         registry_path_t new_path = REGISTRY_PATH();
    //         registry_load_by_path(&new_path);
    //     }

    //     return 0;
    // }
    // else if (strcmp(argv[1], "save") == 0) {
    //     if (argc > 2) {
    //         if (_parse_string_path(argv[2], &path) < 0) {
    //             printf("usage: %s %s [path]\n", argv[0], argv[1]);
    //             return 1;
    //         }
    //         else {
    //             registry_save_by_path(&path);
    //         }
    //     }
    //     else {
    //         registry_path_t new_path = REGISTRY_PATH();
    //         registry_save_by_path(&new_path);
    //     }

    //     return 0;
    // }
#endif

help_error:
    printf("usage: %s {get|set|commit|export|load|save}\n", argv[0]);

    return 1;
}

SHELL_COMMAND(registry, "Registry cli", _registry);
