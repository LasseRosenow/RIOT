/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       RIOT Registry example application
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <string.h>
#include <stdio.h>

#include "msg.h"
#include "shell.h"
#include "board.h"
#include "mtd.h"
#include "registry.h"
#include "registry/cli.h"
#include "registry/schemas.h"
#include "registry/storage.h"

// #define SHELL_QUEUE_SIZE (8)
// static msg_t _shell_queue[SHELL_QUEUE_SIZE];

// static const shell_command_t shell_commands[] = {
//     { "registry", "Registry cli", registry_cli_cmd },
//     { NULL, NULL, NULL }
// };

// int rgb_led_instance_0_commit_cb(const registry_id_t *id, const void *context)
// {
//     (void)context;
//     printf("RGB instance commit_cb was executed on ");

//     if (id != NULL) {
//         printf("param: /%d", *id);
//     }
//     else {
//         printf("whole instance");
//     }

//     printf("\n");

//     return 0;
// }

// registry_schema_rgb_led_t rgb_led_instance_0_data = {
//     .red = 0,
//     .green = 255,
//     .blue = 70,
// };
// registry_instance_t rgb_led_instance_0 = {
//     .name = "rgb-0",
//     .data = &rgb_led_instance_0_data,
//     .commit_cb = &rgb_led_instance_0_commit_cb,
// };

// registry_schema_rgb_led_t rgb_led_instance_1_data = {
//     .red = 90,
//     .green = 4,
//     .blue = 0,
// };
// registry_instance_t rgb_led_instance_1 = {
//     .name = "rgb-1",
//     .data = &rgb_led_instance_1_data,
//     .commit_cb = &rgb_led_instance_0_commit_cb,
// };

// // Littlefs2
// #if IS_USED(MODULE_LITTLEFS2)
// #include "fs/littlefs2_fs.h"
// #define FS_DRIVER littlefs2_file_system
// static littlefs2_desc_t fs_desc = {
//     .lock = MUTEX_INIT,
// };
// #elif IS_USED(MODULE_FATFS_VFS)
// #include "fs/fatfs.h"
// #define FS_DRIVER fatfs_file_system
// static fatfs_desc_t fs_desc;
// #endif



// static vfs_mount_t _vfs_mount = {
//     .fs = &FS_DRIVER,
//     .mount_point = "/sda",
//     .private_data = &fs_desc,
// };

// static registry_storage_instance_t vfs_instance_1 = {
//     .itf = &registry_storage_vfs,
//     .data = &_vfs_mount,
// };

// static registry_storage_instance_t vfs_instance_2 = {
//     .itf = &registry_storage_vfs,
//     .data = &_vfs_mount,
// };

// int main(void)
// {
//     /* init schemas */
//     registry_schemas_init();
//     registry_register_schema_instance(REGISTRY_NAMESPACE_SYS, registry_schema_rgb_led.id,
//                                       &rgb_led_instance_0);
//     registry_register_schema_instance(REGISTRY_NAMESPACE_SYS, registry_schema_rgb_led.id,
//                                       &rgb_led_instance_1);

//     /* init storage_facilities */
//     if (IS_USED(MODULE_LITTLEFS2)) {
//         fs_desc.dev = MTD_0;
//     }
//     registry_register_storage_src(&vfs_instance_1);
//     registry_register_storage_dst(&vfs_instance_2);

//     /* init CLI */
//     msg_init_queue(_shell_queue, SHELL_QUEUE_SIZE);
//     char line_buf[SHELL_DEFAULT_BUFSIZE];

//     shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
//     return 0;
// }



int registry_get_uint8_v2(uint8_t **val, size_t *val_len)
{
    (void)val;
    (void)val_len;

    return 0;
}


// typedef struct { struct { registry_id_t id; } meta;
//                  struct { int (*get)(uint8_t **val, size_t *val_len); } red;
//                  struct { int (*get)(uint8_t **val, size_t *val_len); } green;
//                  struct { int (*get)(uint8_t **val, size_t *val_len); } blue;
//                  struct { _CALL_MACRO_FOR_EACH(
//                               _REGISTRY_SCHEMA_ITEM_DECLARATION_V2,
//                               (0, white, uint8_t),
//                               (0,
//                                yellow, uint8_t)) } brightnesses; } registry_schema_rgb_t;

// registry_schema_rgb_t registry_schema_rgb = {
//     .meta = {
//         .id = 0,
//     },
//     .brightnesses = {
//         _CALL_MACRO_FOR_EACH(
//             _REGISTRY_SCHEMA_ITEM_INITIALIZATION_V2,
//             (0, white, uint8_t), (0, yellow, uint8_t)
//             )
//     }
// };

// typedef struct {
//     int a;
//     int b;
// } my_type_t;

// my_type_t asdf = {
//     .a = 7,
//     .b = 8,
// };

static void mapping(const registry_id_t param_id, const registry_instance_t *instance,
                    void **val,
                    size_t *val_len)
{
    registry_schema_rgb_led_t *_instance = (registry_schema_rgb_led_t *)instance->data;

    switch (param_id) {
    case REGISTRY_SCHEMA_RGB_LED_RED:
        *val = &_instance->red;
        *val_len = sizeof(_instance->red);
        break;

    case REGISTRY_SCHEMA_RGB_LED_GREEN:
        *val = &_instance->green;
        *val_len = sizeof(_instance->green);
        break;

    case REGISTRY_SCHEMA_RGB_LED_BLUE:
        *val = &_instance->blue;
        *val_len = sizeof(_instance->blue);
        break;
    }
}

typedef const struct {
    const registry_schema_data_v2_t data;
    const struct {
        const registry_schema_item_data_v2_t data;
        int (*get)(uint8_t **val, size_t *val_len);
    } red;
    const struct {
        const registry_schema_item_data_v2_t data;
        int (*get)(uint8_t **val, size_t *val_len);
    } green;
    const struct {
        const registry_schema_item_data_v2_t data;
        int (*get)(uint8_t **val, size_t *val_len);
    } blue;
    const struct {
        const registry_schema_item_data_v2_t data;
        const struct {
            const registry_schema_item_data_v2_t data;
            int (*get)(uint8_t **val, size_t *val_len);
        } white;
        const struct {
            const registry_schema_item_data_v2_t data;
            int (*get)(uint8_t **val, size_t *val_len);
        } yellow;
    } brightnesses;
} registry_schema_rgb_v2_t;

registry_schema_rgb_v2_t registry_schema_rgb_v2 = {
    .data = {
        .id = 0,
        .name = "rgb_v2",
        .description = "",
        .mapping = mapping,
    },
    .red = {
        .data = {
            .id = 0,
            .name = "red",
            .description = "",
            .type = REGISTRY_TYPE_UINT8_V2,
        },
        .get = registry_get_uint8_v2,
    },
    .green = {
        .data = {
            .id = 1,
            .name = "green",
            .description = "",
            .type = REGISTRY_TYPE_UINT8_V2,
        },
        .get = registry_get_uint8_v2,
    },
    .blue = {
        .data = {
            .id = 2,
            .name = "blue",
            .description = "",
            .type = REGISTRY_TYPE_UINT8_V2,
        },
        .get = registry_get_uint8_v2,
    },
    .brightnesses = {
        .data = {
            .id = 3,
            .name = "brightnesses",
            .description = "",
            .type = REGISTRY_TYPE_GROUP_V2,
        },
        .white = {
            .data = {
                .id = 4,
                .name = "white",
                .description = "",
                .type = REGISTRY_TYPE_UINT8_V2,
            },
            .get = registry_get_uint8_v2,
        },
        .yellow = {
            .data = {
                .id = 5,
                .name = "yellow",
                .description = "",
                .type = REGISTRY_TYPE_UINT8_V2,
            },
            .get = registry_get_uint8_v2,
        },
    },
};

registry_path_schema_v2_t registry_path_schema_rgb_v2 = {
    .data = &registry_schema_rgb_v2.data,
    .items = (registry_path_schema_item_v2_t[]) {
        {
            .data = &registry_schema_rgb_v2.red.data,
            .items = ((void *)0),
            .items_len = 0,
        },
        {
            .data = &registry_schema_rgb_v2.green.data,
            .items = ((void *)0),
            .items_len = 0,
        },
        {
            .data = &registry_schema_rgb_v2.blue.data,
            .items = ((void *)0),
            .items_len = 0,
        },
        {
            .data = &registry_schema_rgb_v2.brightnesses.data,
            .items = (registry_path_schema_item_v2_t[]) {
                {
                    .data = &registry_schema_rgb_v2.brightnesses.white.data,
                    .items = ((void *)0),
                    .items_len = 0,
                },
                {
                    .data = &registry_schema_rgb_v2.brightnesses.yellow.data,
                    .items = ((void *)0),
                    .items_len = 0,
                },
            },
            .items_len = 0,
        },
    },
    .items_len = 0,
};

int main(void)
{
    // REGISTRY_SCHEMA_V2(
    //     rgb_v2, 0, "", mapping,

    //     REGISTRY_PARAMETER_V2(red, 0, REGISTRY_TYPE_UINT8_V2, uint8_t, ""),
    //     REGISTRY_PARAMETER_V2(green, 1, REGISTRY_TYPE_UINT8_V2, uint8_t, ""),
    //     REGISTRY_PARAMETER_V2(blue, 2,  REGISTRY_TYPE_UINT8_V2, uint8_t, ""),

    //     REGISTRY_GROUP_V2(
    //         brightnesses,
    //         3,
    //         "",

    //         REGISTRY_PARAMETER_V2(white, 4, REGISTRY_TYPE_UINT8_V2, uint8_t, ""),
    //         REGISTRY_PARAMETER_V2(yellow, 5, REGISTRY_TYPE_UINT8_V2, uint8_t, "")
    //         )
    //     );

    // uint8_t *val;
    // size_t val_len;

    // registry_schema_rgb.blue.get(&val, &val_len);

    /* test registry */
    // registry_tests_api_run();
    // registry_tests_stack_run();

    /* run demo app */
    // demo_app();

    return 0;
}
