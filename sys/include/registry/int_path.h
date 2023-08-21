/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_int_path RIOT Registry integer path
 * @ingroup     sys
 * @brief       RIOT Registry integer path module providing functions to convert between registry objects and their integer paths
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
 * Path: namespace_id / schema_id / instance_id / (group_id | parameter_id).
 */
#define REGISTRY_INT_PATH_MAX_LEN 4

/**
 * @brief Maximum length of a configuration path as a string.
 *
 * A int path ID is an uint32_t and uint32_t MAX has 10 digits.
 * We also need to include the separator. One additional char between each number.
 */
#define REGISTRY_INT_PATH_STRING_MAX_LEN   ((10 * REGISTRY_INT_PATH_MAX_LEN) + \
                                            (REGISTRY_INT_PATH_MAX_LEN - 1))

/**
 * @brief Integer path representation for a namespace.
 */
typedef struct {
    registry_namespace_id_t namespace_id; /**< The ID of the namespace. */
} registry_namespace_int_path_t;

/**
 * @brief Integer path representation for a configuration schema.
 */
typedef struct {
    registry_namespace_id_t namespace_id;   /**< The ID of the namespace. */
    registry_schema_id_t schema_id;         /**< The ID of the schema. */
} registry_schema_int_path_t;

/**
 * @brief Integer path representation for a configuration schema instance.
 */
typedef struct {
    registry_namespace_id_t namespace_id;   /**< The ID of the namespace. */
    registry_schema_id_t schema_id;         /**< The ID of the schema. */
    registry_instance_id_t instance_id;     /**< The ID of the instance. */
} registry_instance_int_path_t;

/**
 * @brief Integer path representation for a configuration group.
 */
typedef struct {
    registry_namespace_id_t namespace_id;   /**< The ID of the namespace. */
    registry_schema_id_t schema_id;         /**< The ID of the schema. */
    registry_instance_id_t instance_id;     /**< The ID of the instance. */
    registry_group_id_t group_id;           /**< The ID of the group. */
} registry_group_int_path_t;

/**
 * @brief Integer path representation for a configuration parameter.
 */
typedef struct {
    registry_namespace_id_t namespace_id;   /**< The ID of the namespace. */
    registry_schema_id_t schema_id;         /**< The ID of the schema. */
    registry_instance_id_t instance_id;     /**< The ID of the instance. */
    registry_parameter_id_t parameter_id;   /**< The ID of the parameter. */
} registry_parameter_int_path_t;

/**
 * @brief Integer path representation for a configuration group or parameter.
 */
typedef struct {
    registry_namespace_id_t namespace_id;                   /**< The ID of the namespace. */
    registry_schema_id_t schema_id;                         /**< The ID of the schema. */
    registry_instance_id_t instance_id;                     /**< The ID of the instance. */
    registry_group_or_parameter_id_t group_or_parameter_id; /**< The ID of the group or parameter. */
} registry_group_or_parameter_int_path_t;

/**
 * @brief Enumeration of the different types of integer paths.
 */
typedef enum {
    REGISTRY_INT_PATH_TYPE_NAMESPACE,           /**< The path represents a namespace. */
    REGISTRY_INT_PATH_TYPE_SCHEMA,              /**< The path represents a schema. */
    REGISTRY_INT_PATH_TYPE_INSTANCE,            /**< The path represents an instance. */
    REGISTRY_INT_PATH_TYPE_GROUP,               /**< The path represents a group. */
    REGISTRY_INT_PATH_TYPE_PARAMETER,           /**< The path represents a parameter. */
    REGISTRY_INT_PATH_TYPE_GROUP_OR_PARAMETER,  /**< The path represents a group or parameter. */
}registry_int_path_type_t;

/**
 * @brief Union of the different types of integer paths.
 */
typedef union {
    registry_namespace_int_path_t namespace_path;                   /**< The path represents a namespace. */
    registry_schema_int_path_t schema_path;                         /**< The path represents a schema. */
    registry_instance_int_path_t instance_path;                     /**< The path represents an instance. */
    registry_group_int_path_t group_path;                           /**< The path represents a group. */
    registry_parameter_int_path_t parameter_path;                   /**< The path represents a parameter. */
    registry_group_or_parameter_int_path_t group_or_parameter_path; /**< The path represents a group or parameter. */
} registry_int_path_t;

/**
 * @brief Converts a registry namespace to an integer path.
 *
 * @param[in] namespace The registry namespace to convert.
 *
 * @return The integer path representation of the namespace.
 */
registry_namespace_int_path_t registry_to_namespace_int_path(const registry_namespace_t *namespace);

/**
 * @brief Converts a registry schema to an integer path.
 *
 * @param[in] schema The registry schema to convert.
 *
 * @return The integer path representation of the schema.
 */
registry_schema_int_path_t registry_to_schema_int_path(const registry_schema_t *schema);

/**
 * @brief Converts a registry instance to an integer path.
 *
 * @param[in] instance The registry instance to convert.
 *
 * @return The integer path representation of the instance.
 */
registry_instance_int_path_t registry_to_instance_int_path(const registry_instance_t *instance);

/**
 * @brief Converts a registry group to an integer path.
 *
 * @param[in] instance The registry instance that the group belongs to.
 * @param[in] group The registry group to convert.
 *
 * @return The integer path representation of the group.
 */
registry_group_int_path_t registry_to_group_int_path(const registry_instance_t *instance,
                                                     const registry_group_t *group);

/**
 * @brief Converts a registry parameter to an integer path.
 *
 * @param[in] instance The registry instance that the parameter belongs to.
 * @param[in] parameter The registry parameter to convert.
 *
 * @return The integer path representation of the parameter.
 */
registry_parameter_int_path_t registry_to_parameter_int_path(const registry_instance_t *instance,
                                                             const registry_parameter_t *parameter);

/**
 * @brief Converts an integer path to a registry namespace.
 *
 * @param[in] path The integer path to convert.
 * @param[out] namespace A pointer to the registry namespace that will be set to the result of the conversion.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_namespace_int_path(const registry_namespace_int_path_t *path,
                                     registry_namespace_t **namespace);

/**
 * @brief Converts an integer path to a registry schema.
 *
 * @param[in] path The integer path to convert.
 * @param[out] namespace A pointer to the registry namespace that will be set to the result of the conversion.
 * @param[out] schema A pointer to the registry schema that will be set to the result of the conversion.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_schema_int_path(const registry_schema_int_path_t *path,
                                  registry_namespace_t **namespace, registry_schema_t **schema);

/**
 * @brief Converts an integer path to a registry instance.
 *
 * @param[in] path The integer path to convert.
 * @param[out] namespace A pointer to the registry namespace that will be set to the result of the conversion.
 * @param[out] schema A pointer to the registry schema that will be set to the result of the conversion.
 * @param[out] instance A pointer to the registry instance that will be set to the result of the conversion.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_instance_int_path(const registry_instance_int_path_t *path,
                                    registry_namespace_t **namespace, registry_schema_t **schema,
                                    registry_instance_t **instance);

/**
 * @brief Converts an integer path to a registry group.
 *
 * @param[in] path The integer path to convert.
 * @param[out] namespace A pointer to the registry namespace that will be set to the result of the conversion.
 * @param[out] schema A pointer to the registry schema that will be set to the result of the conversion.
 * @param[out] instance A pointer to the registry instance that will be set to the result of the conversion.
 * @param[out] group A pointer to the registry group that will be set to the result of the conversion.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_group_int_path(const registry_group_int_path_t *path,
                                 registry_namespace_t **namespace, registry_schema_t **schema,
                                 registry_instance_t **instance, registry_group_t **group);

/**
 * @brief Converts an integer path to a registry parameter.
 *
 * @param[in] path The integer path to convert.
 * @param[out] namespace A pointer to the registry namespace that will be set to the result of the conversion.
 * @param[out] schema A pointer to the registry schema that will be set to the result of the conversion.
 * @param[out] instance A pointer to the registry instance that will be set to the result of the conversion.
 * @param[out] parameter A pointer to the registry parameter that will be set to the result of the conversion.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_parameter_int_path(const registry_parameter_int_path_t *path,
                                     registry_namespace_t **namespace, registry_schema_t **schema,
                                     registry_instance_t **instance,
                                     registry_parameter_t **parameter);

/**
 * @brief Converts an integer path to either a registry group or a registry parameter.
 *
 * @param[in] path The integer path to convert.
 * @param[out] path_type A pointer to the type of the integer path that was converted.
 * @param[out] namespace A pointer to the registry namespace that will be set to the result of the conversion.
 * @param[out] schema A pointer to the registry schema that will be set to the result of the conversion.
 * @param[out] instance A pointer to the registry instance that will be set to the result of the conversion.
 * @param[out] group A pointer to the registry group that will be set to the result of the conversion if the integer path represents a group.
 * @param[out] parameter A pointer to the registry parameter that will be set to the result of the conversion if the integer path represents a parameter.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_from_group_or_parameter_int_path(const registry_group_or_parameter_int_path_t *path,
                                              registry_int_path_type_t *path_type,
                                              registry_namespace_t **namespace,
                                              registry_schema_t **schema,
                                              registry_instance_t **instance,
                                              registry_group_t **group,
                                              registry_parameter_t **parameter);

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_INT_PATH_H */
/** @} */
