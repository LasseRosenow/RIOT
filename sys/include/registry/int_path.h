/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_int_path RIOT Registry Int Path
 * @ingroup     sys
 * @brief       RIOT Registry Int Path module providing a API to access configuration parameter via an integer path
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_INT_PATH_H
#define REGISTRY_INT_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"

/**
 * @brief Maximum length of a configuration path.
 *
 * Path: namespace_id / schema_id / instance_id / (group_id | parameter_id)
 */
#define REGISTRY_INT_PATH_LEN 4

/**
 * @brief Maximum length of a configuration path as a string.
 *
 * A path is an uint32_t and uint32_t MAX has 10 digits.
 * We also need to include the separator. One additional char between each number.
 */
#define REGISTRY_INT_PATH_STRING_MAX_LEN   ((10 * REGISTRY_INT_PATH_LEN) + \
                                            (REGISTRY_INT_PATH_LEN - 1))

typedef struct {
    registry_namespace_id_t namespace_id;
} registry_namespace_int_path_t;

typedef struct {
    registry_namespace_id_t namespace_id;
    registry_schema_id_t schema_id;
} registry_schema_int_path_t;

typedef struct {
    registry_namespace_id_t namespace_id;
    registry_schema_id_t schema_id;
    registry_instance_id_t instance_id;
} registry_instance_int_path_t;

typedef struct {
    registry_namespace_id_t namespace_id;
    registry_schema_id_t schema_id;
    registry_instance_id_t instance_id;
    registry_group_id_t group_id;
} registry_group_int_path_t;

typedef struct {
    registry_namespace_id_t namespace_id;
    registry_schema_id_t schema_id;
    registry_instance_id_t instance_id;
    registry_parameter_id_t parameter_id;
} registry_parameter_int_path_t;

typedef enum {
    REGISTRY_INT_PATH_TYPE_NAMESPACE,
    REGISTRY_INT_PATH_TYPE_SCHEMA,
    REGISTRY_INT_PATH_TYPE_INSTANCE,
    REGISTRY_INT_PATH_TYPE_GROUP,
    REGISTRY_INT_PATH_TYPE_PARAMETER,
}registry_int_path_type_t;

typedef union {
    registry_namespace_int_path_t namespace_path;
    registry_schema_int_path_t schema_path;
    registry_instance_int_path_t instance_path;
    registry_group_int_path_t group_path;
    registry_parameter_int_path_t parameter_path;
} registry_int_path_t;

/* to int path */
registry_namespace_int_path_t registry_to_namespace_int_path(const registry_namespace_t *namespace);

registry_schema_int_path_t registry_to_schema_int_path(const registry_schema_t *schema);

registry_instance_int_path_t registry_to_instance_int_path(const registry_instance_t *instance);

registry_group_int_path_t registry_to_group_int_path(const registry_instance_t *instance,
                                                     const registry_group_t *group);

registry_parameter_int_path_t registry_to_parameter_int_path(const registry_instance_t *instance,
                                                             const registry_parameter_t *parameter);

/* from int path */
int registry_from_namespace_int_path(const registry_namespace_int_path_t *path,
                                     registry_namespace_t **namespace);

int registry_from_schema_int_path(const registry_schema_int_path_t *path,
                                  registry_namespace_t **namespace, registry_schema_t **schema);

int registry_from_instance_int_path(const registry_instance_int_path_t *path,
                                    registry_namespace_t **namespace, registry_schema_t **schema,
                                    registry_instance_t **instance);

int registry_from_group_int_path(const registry_group_int_path_t *path,
                                 registry_namespace_t **namespace, registry_schema_t **schema,
                                 registry_instance_t **instance, registry_group_t **group);

int registry_from_parameter_int_path(const registry_parameter_int_path_t *path,
                                     registry_namespace_t **namespace, registry_schema_t **schema,
                                     registry_instance_t **instance,
                                     registry_parameter_t **parameter);

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_INT_PATH_H */
/** @} */
