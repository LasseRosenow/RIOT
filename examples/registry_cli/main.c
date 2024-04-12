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
 * @brief       RIOT registry CLI example application to demonstrate how
 *              to use the RIOT registry using the CLI.
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
#include "vfs.h"
#include "fs/littlefs2_fs.h"
#include "registry.h"
#include "registry/namespace/sys.h"
#include "registry/namespace/sys/rgb_led.h"
#include "registry/storage.h"
#include "registry/string_path.h"

/* this callback is usually implemented drivers such as an RGB LED driver */
static int shared_commit_cb(const registry_commit_cb_scope_t scope,
                            const registry_group_or_parameter_id_t *group_or_parameter_id,
                            const void *context)
{
    /* since this function is shared by multiple commit_cb functions, we path the instance in the context */
    const registry_instance_t *instance = context;
    const registry_sys_rgb_led_instance_t *data = instance->data;

    printf("RGB instance commit_cb was executed on ");

    /* check how much of the registry instance was committed and print the new values */
    switch (scope)
    {
    case REGISTRY_COMMIT_INSTANCE:
        /* in this case the whole instance and all parameters inside of it was committed */
        printf("the whole instance: ID: %d\n", instance->id);
        printf("\tParameter ID: 0, VALUE: %d\n", data->red);
        printf("\tParameter ID: 1, VALUE: %d\n", data->green);
        printf("\tParameter ID: 2, VALUE: %d\n", data->blue);
        break;
    case REGISTRY_COMMIT_GROUP:
        /* in this case a group and all parameters inside of it was committed */
        printf("a group: %d\n", *group_or_parameter_id);
        /* this instance does not have groups, so no need to print anything here */
        break;
    case REGISTRY_COMMIT_PARAMETER:
        /* in this case only a single parameter was committed */
        printf("a single parameter: ID: %d,", *group_or_parameter_id);
        switch (*group_or_parameter_id)
        {
        case 0: printf(" VALUE: %d\n", data->red); break;
        case 1: printf(" VALUE: %d\n", data->green); break;
        case 2: printf(" VALUE: %d\n", data->blue); break;
        }
    }

    return 0;
}

/* create instances of a configuration schema, to expose configuration parameters to the registry */
static int rgb_led_instance_0_commit_cb(const registry_commit_cb_scope_t scope,
                                 const registry_group_or_parameter_id_t *group_or_parameter_id,
                                 const void *context);
static registry_sys_rgb_led_instance_t rgb_led_instance_0_data = {
    .red = 0,
    .green = 255,
    .blue = 70,
};
static registry_instance_t rgb_led_instance_0 = {
    .name = "rgb-0",
    .data = &rgb_led_instance_0_data,
    .commit_cb = &rgb_led_instance_0_commit_cb,
};
static int rgb_led_instance_0_commit_cb(const registry_commit_cb_scope_t scope,
                                 const registry_group_or_parameter_id_t *group_or_parameter_id,
                                 const void *context) 
{
    (void)context;
    return shared_commit_cb(scope, group_or_parameter_id, &rgb_led_instance_0);
}

/* create an instance for a second RGB LED */
static int rgb_led_instance_1_commit_cb(const registry_commit_cb_scope_t scope,
                                 const registry_group_or_parameter_id_t *group_or_parameter_id,
                                 const void *context);
static registry_sys_rgb_led_instance_t rgb_led_instance_1_data = {
    .red = 90,
    .green = 4,
    .blue = 0,
};
static registry_instance_t rgb_led_instance_1 = {
    .name = "rgb-1",
    .data = &rgb_led_instance_1_data,
    .commit_cb = &rgb_led_instance_1_commit_cb,
};
static int rgb_led_instance_1_commit_cb(const registry_commit_cb_scope_t scope,
                                 const registry_group_or_parameter_id_t *group_or_parameter_id,
                                 const void *context)
{
    (void)context;
    return shared_commit_cb(scope, group_or_parameter_id, &rgb_led_instance_1);
}

/* configure the registry storage to use littlefs2 */
static littlefs2_desc_t fs_desc = {
    .lock = MUTEX_INIT,
};

/* set the mount point for the registry storage to /sda for this example */
static vfs_mount_t _vfs_mount = {
    .fs = &littlefs2_file_system,
    .mount_point = "/sda",
    .private_data = &fs_desc,
};

/* create a storage instance to register the storage at the RIOT registry */
static registry_storage_instance_t vfs_instance = {
    .storage = &registry_storage_vfs,
    .data = &_vfs_mount,
};

/* the storage source is where the registry reads parameter values from */
REGISTRY_ADD_STORAGE_SOURCE(vfs_instance);

/* the storage destination is where the registry writes parameter values to */
REGISTRY_SET_STORAGE_DESTINATION(vfs_instance);

int main(void)
{
    /* initialize the riot registry storage for persistent configuration parameters */
    #if IS_USED(MODULE_LITTLEFS2)
    fs_desc.dev = MTD_0;
    #endif

    /* initialize the riot registry */
    registry_init();

    /* add configuration schemas to the registry */
    registry_add_schema_instance(&registry_sys_rgb_led, &rgb_led_instance_0);
    registry_add_schema_instance(&registry_sys_rgb_led, &rgb_led_instance_1);

    /* initialize and run the RIOT shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, sizeof(line_buf));
    return 0;
}
