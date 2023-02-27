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
#include "registry.h"
#include "registry/cli.h"
#include "registry/schemas.h"

#define SHELL_QUEUE_SIZE (8)
static msg_t _shell_queue[SHELL_QUEUE_SIZE];

static const shell_command_t shell_commands[] = {
    { "registry", "Registry cli", registry_cli_cmd },
    { NULL, NULL, NULL }
};

int main(void)
{
    registry_init();
    registry_schemas_init();

    /* Init CLI */
    msg_init_queue(_shell_queue, SHELL_QUEUE_SIZE);
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}

// #include "registry.h"
// #include "registry_schemas.h"
// #include "registry_cli.h"
// #include "registry_tests.h"
// #include "registry_storage_facilities.h"
// #include "assert.h"
// #include "vfs.h"
// #include "board.h"
// #include "mtd.h"
// #include "ps.h"
// // #include "ws281x.h"
// // #include "ws281x_params.h"

// #define SHELL_QUEUE_SIZE (8)
// static msg_t _shell_queue[SHELL_QUEUE_SIZE];

// static const shell_command_t shell_commands[] = {
//     { "registry", "Registry cli", registry_cli_cmd },
//     { NULL, NULL, NULL }
// };

// // ws281x_t dev;

// int rgb_led_instance_0_commit_cb(const registry_path_t path, const void *context)
// {
//     (void)context;
//     printf("RGB instance commit_cb was executed: %d", *path.namespace_id);
//     if (path.schema_id) {
//         printf("/%d", *path.schema_id);
//     }
//     if (path.instance_id) {
//         printf("/%d", *path.instance_id);
//     }
//     printf("\n");

//     // uint8_t red =
//     //     registry_get_uint8(REGISTRY_PATH_SYS(*path.schema_id, *path.instance_id,
//     //                                          REGISTRY_SCHEMA_RGB_LED_RED));
//     // uint8_t green =
//     //     registry_get_uint8(REGISTRY_PATH_SYS(*path.schema_id, *path.instance_id,
//     //                                          REGISTRY_SCHEMA_RGB_LED_GREEN));
//     // uint8_t blue =
//     //     registry_get_uint8(REGISTRY_PATH_SYS(*path.schema_id, *path.instance_id,
//     //                                          REGISTRY_SCHEMA_RGB_LED_BLUE));

//     // color_rgb_t color = { .r = red, .g = green, .b = blue };

//     // ws281x_set(&dev, 0, color);
//     // ws281x_write(&dev);

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

// registry_schema_rgb_led_t rgb_led_instance_2_data = {
//     .red = 7,
//     .green = 8,
//     .blue = 9,
// };
// registry_instance_t rgb_led_instance_2 = {
//     .name = "rgb-2",
//     .data = &rgb_led_instance_2_data,
//     .commit_cb = &rgb_led_instance_0_commit_cb,
// };

// registry_schema_rgb_led_t rgb_led_instance_3_data = {
//     .red = 7,
//     .green = 8,
//     .blue = 9,
// };
// registry_instance_t rgb_led_instance_3 = {
//     .name = "rgb-3",
//     .data = &rgb_led_instance_3_data,
//     .commit_cb = &rgb_led_instance_0_commit_cb,
// };

// registry_schema_rgb_led_t rgb_led_instance_4_data = {
//     .red = 7,
//     .green = 8,
//     .blue = 9,
// };
// registry_instance_t rgb_led_instance_4 = {
//     .name = "rgb-4",
//     .data = &rgb_led_instance_4_data,
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

// static registry_storage_facility_instance_t vfs_instance_1 = {
//     .itf = &registry_storage_facility_vfs,
//     .data = &_vfs_mount,
// };

// static registry_storage_facility_instance_t vfs_instance_2 = {
//     .itf = &registry_storage_facility_vfs,
//     .data = &_vfs_mount,
// };

// int demo_app(void)
// {
//     /* init registry */
//     registry_init();

//     /* init schemas */
//     registry_schemas_init();
//     registry_register_schema_instance(REGISTRY_ROOT_GROUP_SYS, registry_schema_rgb_led.id,
//                                       &rgb_led_instance_0);
//     registry_register_schema_instance(REGISTRY_ROOT_GROUP_SYS, registry_schema_rgb_led.id,
//                                       &rgb_led_instance_1);
//     registry_register_schema_instance(REGISTRY_ROOT_GROUP_SYS, registry_schema_rgb_led.id,
//                                       &rgb_led_instance_2);

//     /* init storage_facilities */
//     if (IS_USED(MODULE_LITTLEFS2)) {
//         fs_desc.dev = MTD_0;
//     }
//     registry_register_storage_facility_src(&vfs_instance_1);
//     registry_register_storage_facility_dst(&vfs_instance_2);

//     /* load old storage_facility data into registry */
//     registry_load(_REGISTRY_PATH_0());

//     /* DEMO START */
//     // int retval;

//     // if (0 != (retval = ws281x_init(&dev, &ws281x_params[0]))) {
//     //     printf("Initialization failed with error code %d\n", retval);
//     //     return retval;
//     // }

//     // uint8_t red =
//     //     registry_get_uint8(REGISTRY_PATH_SYS(registry_schema_rgb_led.id, 0,
//     //                                          REGISTRY_SCHEMA_RGB_LED_RED));
//     // uint8_t green =
//     //     registry_get_uint8(REGISTRY_PATH_SYS(registry_schema_rgb_led.id, 0,
//     //                                          REGISTRY_SCHEMA_RGB_LED_GREEN));
//     // uint8_t blue =
//     //     registry_get_uint8(REGISTRY_PATH_SYS(registry_schema_rgb_led.id, 0,
//     //                                          REGISTRY_SCHEMA_RGB_LED_BLUE));

//     // color_rgb_t color = { .r = red, .g = green, .b = blue };

//     // ws281x_set(&dev, 0, color);
//     // ws281x_write(&dev);
//     // DEMO END

//     /* Init CLI */
//     msg_init_queue(_shell_queue, SHELL_QUEUE_SIZE);
//     char line_buf[SHELL_DEFAULT_BUFSIZE];

//     shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
//     return 0;
// }





// int registry_get_uint8_v2(uint8_t **val, size_t *val_len)
// {
//     (void)val;
//     (void)val_len;

//     return 0;
// }


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

// int main(void)
// {
//     registry_init();
//     // REGISTRY_SCHEMA_V2(
//     //     rgb, 0,

//     //     REGISTRY_PARAMETER_V2(red, uint8_t),
//     //     REGISTRY_PARAMETER_V2(green, uint8_t),
//     //     REGISTRY_PARAMETER_V2(blue, uint8_t),

//     //     REGISTRY_GROUP_V2(
//     //         brightnesses,

//     //         REGISTRY_PARAMETER_V2(white, uint8_t),
//     //         REGISTRY_PARAMETER_V2(yellow, uint8_t)
//     //         )
//     //     );

//     // uint8_t *val;
//     // size_t val_len;

//     // registry_schema_rgb.blue.get(&val, &val_len);

//     /* test registry */
//     // registry_tests_api_run();
//     // registry_tests_stack_run();

//     /* run demo app */
//     // demo_app();

//     return 0;
// }
