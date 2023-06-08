/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_path_storage_vfs RIOT Registry Path Storage: VFS
 * @ingroup     sys
 * @brief       RIOT Registry Path Storage VFS, allows using the RIOT VFS module as a RIOT Registry data storage.
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "vfs.h"
#include "ps.h"
#include "registry/path.h"

#include "registry/storage.h"

static int load(const registry_storage_instance_t *storage,
                const load_cb_t load_cb);
static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_resource_t *parameter,
                const registry_value_t *value);

registry_storage_t registry_storage_vfs = {
    .load = load,
    .save = save,
};

static void _string_path_append_item(char *dest, uint32_t number)
{
    int size = snprintf(NULL, 0, "/%d", number);

    char buf[size];

    sprintf(buf, "/%d", number);

    strcat(dest, buf);
}

static int _format(vfs_mount_t *mount)
{
    DEBUG("formatting %s....\t", mount->mount_point);
    if (vfs_format(mount) < 0) {
        DEBUG("[Failed]\n");
        return 1;
    }
    else {
        DEBUG("[OK]\n");
    }

    return 0;
}

static int _mount(vfs_mount_t *mount)
{
    int res = vfs_mount(mount);

    if (res < 0) {
        DEBUG("Error while mounting %s ... (%d) ... try format\n", mount->mount_point, res);

        /* format to fix mount */
        _format(mount);

        /* try to mount again */
        res = vfs_mount(mount);
        if (res != 0) {
            return -1;
        }
    }

    return 0;
}

static int _umount(vfs_mount_t *mount)
{
    int res = vfs_umount(mount, false);

    if (res < 0) {
        DEBUG("Error while unmounting %s...\n", mount->mount_point);
        return 1;
    }

    return 0;
}

static int load(const registry_storage_instance_t *storage,
                const load_cb_t load_cb)
{
    vfs_mount_t *mount = storage->data;

    /* mount */
    _mount(mount);

    /* create dir path */
    char string_path[REGISTRY_PATH_STRING_MAX_LEN];

    sprintf(string_path, "%s", mount->mount_point);

    /* read dirs */
    vfs_DIR dirp;

    if (vfs_opendir(&dirp, string_path) != 0) {
        DEBUG("[registry storage_vfs] load: Can not open dir\n");
    }
    else {
        struct stat _stat;
        vfs_dirent_t dir_entry;

        size_t i = 0;
        int last_dir_entry_positions[REGISTRY_PATH_LEN] = { -1 };
        size_t last_dir_string_path_lens[REGISTRY_PATH_LEN] = { 0 };
        int res = 0;
        bool exit_folder_iteration = false;

        while (exit_folder_iteration == false) {
            int dir_entry_position = -1;
            do {
                res = vfs_readdir(&dirp, &dir_entry);
                dir_entry_position++;

                if (dir_entry_position > last_dir_entry_positions[i]) {
                    last_dir_entry_positions[i] = dir_entry_position;
                    for (size_t j = i + 1; j < REGISTRY_PATH_LEN; j++) {
                        last_dir_entry_positions[j] = -1;
                    }

                    if (res == 1) {
                        if (strcmp(dir_entry.d_name,
                                   ".") != 0 && strcmp(dir_entry.d_name, "..") != 0) {
                            /* save string_path length to restore it later */
                            last_dir_string_path_lens[i] = strlen(string_path);

                            /* add new directory to string_path */
                            strcat(string_path, "/");
                            strcat(string_path, dir_entry.d_name);

                            vfs_stat(string_path, &_stat);

                            if (S_ISDIR(_stat.st_mode)) {
                                /* close old directory */
                                if (vfs_closedir(&dirp) != 0) {
                                    DEBUG(
                                        "[registry storage_vfs] load: Can not close dir\n");
                                }

                                /* open new directory */
                                if (vfs_opendir(&dirp, string_path) != 0) {
                                    DEBUG("[registry storage_vfs] load: Can not open dir\n");
                                }

                                /* move on to next sub path */
                                i++;

                                /* reset position within current dir, because the dir changed */
                                dir_entry_position = -1;
                            }
                            else {
                                /* open file */
                                int fd = vfs_open(string_path, O_RDONLY, 0);

                                if (fd <= 0) {
                                    DEBUG(
                                        "[registry storage_vfs] load: Can not open file: %d\n",
                                        fd);
                                }

                                /* get filesize */
                                vfs_fstat(fd, &_stat);

                                /* try to convert string path to registry int path */
                                registry_path_t path;
                                if (registry_path_from_string(string_path +
                                                              strlen(mount->mount_point),
                                                              &path) < 0) {
                                    DEBUG(
                                        "[registry storage_vfs] load: Invalid registry path\n");
                                }
                                else {
                                    /* get pointer to registry internal configuration parameter */
                                    registry_value_t old_value;
                                    const registry_resource_t *resource =
                                        registry_resource_from_path(
                                            &path);
                                    const registry_instance_t *instance =
                                        registry_instance_from_path(
                                            &path);
                                    registry_get(instance, resource, &old_value);

                                    /* read value from file */
                                    uint8_t new_value_buf[old_value.buf_len];
                                    if (vfs_read(fd, new_value_buf, old_value.buf_len) < 0) {
                                        DEBUG(
                                            "[registry storage_vfs] load: Can not read from file\n");
                                    }
                                    else {
                                        /* call callback with value and path */
                                        load_cb(old_value.buf, new_value_buf, old_value.buf_len);
                                    }
                                }

                                /* close file */
                                if (vfs_close(fd) != 0) {
                                    DEBUG(
                                        "[registry storage_vfs] load: Can not close file: %d\n",
                                        fd);
                                }

                                /* restore old string_path */
                                string_path[last_dir_string_path_lens[i]] = '\0';
                            }
                        }
                    }
                    else {
                        /* if i == 0 it can't be further decreased => exit */
                        if (i == 0) {
                            exit_folder_iteration = true;
                        }
                        else {
                            /* move up one path back to the parent */
                            i--;

                            /* restore old string_path */
                            string_path[last_dir_string_path_lens[i]] = '\0';

                            /* close old directory */
                            if (vfs_closedir(&dirp) != 0) {
                                DEBUG("[registry storage_vfs] load: Can not close dir\n");
                            }

                            /* open new directory */
                            if (vfs_opendir(&dirp, string_path) != 0) {
                                DEBUG("[registry storage_vfs] load: Can not open dir\n");
                            }
                        }
                    }
                }
            } while (res == 1);
        }

        if (vfs_closedir(&dirp) != 0) {
            DEBUG("[registry storage_vfs] load: Can not close dir\n");
        }
    }

    /* umount */
    _umount(mount);

    return 0;
}

static int save(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_resource_t *parameter,
                const registry_value_t *value)
{
    vfs_mount_t *mount = storage->data;

    /* mount */
    _mount(mount);

    /* create dir path */
    registry_path_t path;
    registry_namespace_id_t namespace_id;
    registry_instance_id_t instance_id;
    registry_path_from_resource(instance, parameter, &path, &namespace_id, &instance_id);

    char string_path[REGISTRY_PATH_STRING_MAX_LEN];

    sprintf(string_path, "%s", mount->mount_point);

    _string_path_append_item(string_path, *path.namespace_id);
    int res = vfs_mkdir(string_path, 0);

    if (res < 0 && res != -EEXIST) {
        DEBUG("[registry storage_vfs] save: Can not make dir: %s\n", string_path);
    }

    _string_path_append_item(string_path, *path.schema_id);
    res = vfs_mkdir(string_path, 0);

    if (res < 0 && res != -EEXIST) {
        DEBUG("[registry storage_vfs] save: Can not make dir: %s\n", string_path);
    }

    _string_path_append_item(string_path, *path.instance_id);
    res = vfs_mkdir(string_path, 0);

    if (res < 0 && res != -EEXIST) {
        DEBUG("[registry storage_vfs] save: Can not make dir: %s\n", string_path);
    }

    /* open file */
    _string_path_append_item(string_path, *path.resource_id);

    int fd = vfs_open(string_path, O_CREAT | O_RDWR, 0);

    if (fd <= 0) {
        DEBUG("[registry storage_vfs] save: Can not open file: %d\n", fd);
    }

    if (vfs_write(fd, value->buf, value->buf_len) < 0) {
        DEBUG("[registry storage_vfs] save: Can not write to file: %d\n", fd);
    }

    if (vfs_close(fd) != 0) {
        DEBUG("[registry storage_vfs] save: Can not close file: %d\n", fd);
    }

    /* umount */
    _umount(mount);

    return 0;
}
