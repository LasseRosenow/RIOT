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

typedef struct {
    registry_namespace_id_t namespace_id;
} registry_namespace_path_t;

typedef struct {
    registry_namespace_id_t namespace_id;
    registry_schema_id_t schema_id;
} registry_schema_path_t;

typedef struct {
    registry_namespace_id_t namespace_id;
    registry_schema_id_t schema_id;
    registry_instance_id_t instance_id;
} registry_instance_path_t;

typedef struct {
    registry_namespace_id_t namespace_id;
    registry_schema_id_t schema_id;
    registry_instance_id_t instance_id;
    registry_resource_id_t resource_id;
} registry_resource_path_t;

typedef enum {
    REGISTRY_PATH_TYPE_NAMESPACE,
    REGISTRY_PATH_TYPE_SCHEMA,
    REGISTRY_PATH_TYPE_INSTANCE,
    REGISTRY_PATH_TYPE_RESOURCE,
}registry_path_type_t;

typedef union {
    registry_namespace_path_t namespace_path;
    registry_schema_path_t schema_path;
    registry_instance_path_t instance_path;
    registry_resource_path_t resource_path;
} registry_path_t;

registry_namespace_path_t registry_to_namespace_path(const registry_namespace_t *namespace);

registry_schema_path_t registry_to_schema_path(const registry_schema_t *schema);

registry_instance_path_t registry_to_instance_path(const registry_instance_t *instance);

registry_resource_path_t registry_to_resource_path(const registry_instance_t *instance,
                                                   const registry_resource_t *resource);

/* convert from path */
int registry_from_namespace_path(const registry_namespace_path_t *path,
                                 registry_namespace_t **namespace);

int registry_from_schema_path(const registry_schema_path_t *path,
                              registry_namespace_t **namespace, registry_schema_t **schema);

int registry_from_instance_path(const registry_instance_path_t *path,
                                registry_namespace_t **namespace, registry_schema_t **schema,
                                registry_instance_t **instance);

int registry_from_resource_path(const registry_resource_path_t *path,
                                registry_namespace_t **namespace, registry_schema_t **schema,
                                registry_instance_t **instance, registry_resource_t **resource);

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_PATH_H */
/** @} */
