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
#include "registry/sys_namespace.h"
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

int main(void)
{
    return 0;
}
