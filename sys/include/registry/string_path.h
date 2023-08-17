/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_string_path RIOT Registry String Path
 * @ingroup     sys
 * @brief       RIOT Registry String Path module providing functions to convert between registry objects and their string paths
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_STRING_PATH_H
#define REGISTRY_STRING_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    REGISTRY_STRING_PATH_TYPE_NAMESPACE,
    REGISTRY_STRING_PATH_TYPE_SCHEMA,
    REGISTRY_STRING_PATH_TYPE_INSTANCE,
    REGISTRY_STRING_PATH_TYPE_GROUP,
    REGISTRY_STRING_PATH_TYPE_PARAMETER,
    REGISTRY_STRING_PATH_TYPE_GROUP_OR_PARAMETER,
}registry_string_path_type_t;

#include "registry.h"

/**
 * @brief Converts a registry namespace object to its string path representation.
 *
 * @param[in] namespace The registry namespace object to convert.
 * @param[out] path The buffer to store the resulting string path.
 *
 * @return Length of string path on success, non-zero on failure.
 */
int registry_to_namespace_string_path(const registry_namespace_t *namespace, char *path);

/**
 * @brief Converts a registry schema object to its string path representation.
 *
 * @param[in] schema The registry schema object to convert.
 * @param[out] path The buffer to store the resulting string path.
 *
 * @return Length of string path on success, non-zero on failure.
 */
int registry_to_schema_string_path(const registry_schema_t *schema, char *path);

/**
 * @brief Converts a registry instance object to its string path representation.
 *
 * @param[in] instance The registry instance object to convert.
 * @param[out] path The buffer to store the resulting string path.
 *
 * @return Length of string path on success, non-zero on failure.
 */
int registry_to_instance_string_path(const registry_instance_t *instance, char *path);

/**
 * @brief Converts a registry group object to its string path representation.
 *
 * @param[in] instance The registry instance object to which the group belongs.
 * @param[in] group The registry group object to convert.
 * @param[out] path The buffer to store the resulting string path.
 *
 * @return Length of string path on success, non-zero on failure.
 */
int registry_to_group_string_path(const registry_instance_t *instance,
                                  const registry_group_t *group, char *path);

/**
 * @brief Converts a registry parameter object to its string path representation.
 *
 * @param[in] instance The registry instance object to which the parameter belongs.
 * @param[in] parameter The registry parameter object to convert.
 * @param[out] path The buffer to store the resulting string path.
 *
 * @return Length of string path on success, non-zero on failure.
 */
int registry_to_parameter_string_path(const registry_instance_t *instance,
                                      const registry_parameter_t *parameter, char *path);

/**
 * @brief Converts a string path to a registry namespace object.
 *
 * @param[in] path The string path to convert.
 * @param[out] namespace A pointer to the registry namespace object to be created.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_namespace_string_path(const char *path, registry_namespace_t **namespace);

/**
 * @brief Converts a string path to a registry schema object.
 *
 * @param[in] path The string path to convert.
 * @param[out] namespace A pointer to the registry namespace object to be created.
 * @param[out] schema A pointer to the registry schema object to be created.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_schema_string_path(const char *path, registry_namespace_t **namespace,
                                     registry_schema_t **schema);

/**
 * @brief Converts a string path to a registry instance object.
 *
 * @param[in] path The string path to convert.
 * @param[out] namespace A pointer to the registry namespace object to be created.
 * @param[out] schema A pointer to the registry schema object to be created.
 * @param[out] instance A pointer to the registry instance object to be created.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_instance_string_path(const char *path, registry_namespace_t **namespace,
                                       registry_schema_t **schema, registry_instance_t **instance);

/**
 * @brief Converts a string path to a registry group object.
 *
 * @param[in] path The string path to convert.
 * @param[out] namespace A pointer to the registry namespace object to be created.
 * @param[out] schema A pointer to the registry schema object to be created.
 * @param[out] instance A pointer to the registry instance object to which the group belongs.
 * @param[out] group A pointer to the registry group object to be created.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_group_string_path(const char *path, registry_namespace_t **namespace,
                                    registry_schema_t **schema, registry_instance_t **instance,
                                    registry_group_t **group);

/**
 * @brief Converts a string path to a registry parameter object.
 *
 * @param[in] path The string path to convert.
 * @param[out] namespace A pointer to the registry namespace object to be created.
 * @param[out] schema A pointer to the registry schema object to be created.
 * @param[out] instance A pointer to the registry instance object to which the parameter belongs.
 * @param[out] parameter A pointer to the registry parameter object to be created.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_parameter_string_path(const char *path, registry_namespace_t **namespace,
                                        registry_schema_t **schema, registry_instance_t **instance,
                                        registry_parameter_t **parameter);

/**
 * @brief Converts a string path to either a registry group or a registry parameter object.
 *
 * @param[in] path The string path to convert.
 * @param[out] path_type A pointer to the type of the resulting object.
 * @param[out] namespace A pointer to the registry namespace object to be created.
 * @param[out] schema A pointer to the registry schema object to be created.
 * @param[out] instance A pointer to the registry instance object to which the group or parameter belongs.
 * @param[out] group A pointer to the registry group object to be created.
 * @param[out] parameter A pointer to the registry parameter object to be created.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_group_or_parameter_string_path(const char *path,
                                                 registry_string_path_type_t *path_type,
                                                 registry_namespace_t **namespace,
                                                 registry_schema_t **schema,
                                                 registry_instance_t **instance,
                                                 registry_group_t **group,
                                                 registry_parameter_t **parameter);

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_STRING_PATH_H */
/** @} */
