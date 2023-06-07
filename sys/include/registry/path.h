/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_path RIOT Registry Path
 * @ingroup     sys
 * @brief       RIOT Registry Path module providing a API to access configuration parameter via a integer path
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 * @author      Leandro Lanzieri <leandro.lanzieri@haw-hamburg.de>
 */

#ifndef REGISTRY_PATH_H
#define REGISTRY_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"

/**
 * @brief Maximum length of a configuration path.
 *
 * Path: namespace_id / schema_id / instance_id / resource_id
 */
#define REGISTRY_PATH_LEN 4

/**
 * @brief Maximum length of a configuration path as a string.
 *
 * A path is an uint32_t and uint32_t MAX has 10 digits.
 * We also need to include the seperator. One additional char between each number.
 */
#define REGISTRY_PATH_STRING_MAX_LEN   ((10 * REGISTRY_PATH_LEN) + \
                                        (REGISTRY_PATH_LEN - 1))

/* Dynamic registry path */
typedef struct {
    const registry_id_t *namespace_id;
    const registry_id_t *schema_id;
    const registry_id_t *instance_id;
    const registry_id_t *resource_id;
} registry_path_t;

#define _REGISTRY_PATH_NUMARGS(...)  (sizeof((registry_id_t[]){ __VA_ARGS__ }) / \
                                      sizeof(registry_id_t))

#define _REGISTRY_PATH_0() \
        (registry_path_t) { \
            .namespace_id = NULL, \
            .schema_id = NULL, \
            .instance_id = NULL, \
            .resource_id = NULL, \
        }

#define _REGISTRY_PATH_1(_namespace_id) \
        (registry_path_t) { \
            .namespace_id = (registry_id_t[]) { _namespace_id }, \
            .schema_id = NULL, \
            .instance_id = NULL, \
            .resource_id = NULL, \
        }

#define _REGISTRY_PATH_2(_namespace_id, _schema_id) \
        (registry_path_t) { \
            .namespace_id = (registry_id_t[]) { _namespace_id }, \
            .schema_id = (registry_id_t[]) { _schema_id }, \
            .instance_id = NULL, \
            .resource_id = NULL, \
        }

#define _REGISTRY_PATH_3(_namespace_id, _schema_id, _instance_id) \
        (registry_path_t) { \
            .namespace_id = (registry_id_t[]) { _namespace_id }, \
            .schema_id = (registry_id_t[]) { _schema_id }, \
            .instance_id = (registry_id_t[]) { _instance_id }, \
            .resource_id = NULL, \
        }

#define _REGISTRY_PATH_4(_namespace_id, _schema_id, _instance_id, _resource_id) \
        (registry_path_t) { \
            .namespace_id = (registry_id_t[]) { _namespace_id }, \
            .schema_id = (registry_id_t[]) { _schema_id }, \
            .instance_id = (registry_id_t[]) { _instance_id }, \
            .resource_id = (registry_id_t[]) { _resource_id }, \
        }

#define _REGISTRY_PATH_GET_MACRO(_0, _1, _2, _3, _4, NAME, ...) NAME
#define REGISTRY_PATH(...) \
        _REGISTRY_PATH_GET_MACRO(_0, ## __VA_ARGS__, \
                                 _REGISTRY_PATH_4, \
                                 _REGISTRY_PATH_3, \
                                 _REGISTRY_PATH_2, \
                                 _REGISTRY_PATH_1, \
                                 _REGISTRY_PATH_0) \
        (__VA_ARGS__)


int registry_path_from_namespace(const registry_namespace_t *namespace, registry_path_t *path,
                                 registry_id_t *namespace_id_buf);

int registry_path_from_schema(const registry_schema_t *schema, registry_path_t *path,
                              registry_id_t *namespace_id_buf);

int registry_path_from_instance(const registry_instance_t *instance, registry_path_t *path,
                                registry_id_t *namespace_id_buf, registry_id_t *instance_id_buf);

int registry_path_from_resource(const registry_instance_t *instance,
                                const registry_resource_t *resource, registry_path_t *path,
                                registry_id_t *namespace_id_buf, registry_id_t *instance_id_buf);

/* convert from path */
registry_resource_t *registry_namespace_from_path(const registry_path_t *path);

registry_resource_t *registry_schema_from_path(const registry_path_t *path);

registry_resource_t *registry_instance_from_path(const registry_path_t *path);

registry_resource_t *registry_resource_from_path(const registry_path_t *path);


#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_PATH_H */
/** @} */
