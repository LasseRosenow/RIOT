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

/* Dynamic registry path */
typedef struct {
    const registry_id_t *namespace_id;
    const registry_id_t *schema_id;
    const registry_id_t *instance_id;
    const registry_id_t *path;
    size_t path_len;
} registry_path_t;

/**
 * @brief Descriptor used to check duplications in storage facilities
 */
typedef struct {
    const registry_path_t path; /**< path of the parameter being checked */
    const registry_value_t val; /**< value of the parameter being checked */
    bool is_dup;                /**< flag indicating if the parameter is duplicated or not */
} registry_dup_check_arg_t;

#define _REGISTRY_PATH_NUMARGS(...)  (sizeof((registry_id_t[]){ __VA_ARGS__ }) / \
                                      sizeof(registry_id_t))

#define _REGISTRY_PATH_0() \
        (registry_path_t) { \
            .namespace_id = NULL, \
            .schema_id = NULL, \
            .instance_id = NULL, \
            .path = NULL, \
            .path_len = 0, \
        }

#define _REGISTRY_PATH_1(_namespace_id) \
        (registry_path_t) { \
            .namespace_id = (registry_namespace_id_t[]) { _namespace_id }, \
            .schema_id = NULL, \
            .instance_id = NULL, \
            .path = NULL, \
            .path_len = 0, \
        }

#define _REGISTRY_PATH_2(_namespace_id, _schema_id) \
        (registry_path_t) { \
            .namespace_id = (registry_namespace_id_t[]) { _namespace_id }, \
            .schema_id = (registry_id_t[]) { _schema_id }, \
            .instance_id = NULL, \
            .path = NULL, \
            .path_len = 0, \
        }

#define _REGISTRY_PATH_3(_namespace_id, _schema_id, _instance_id) \
        (registry_path_t) { \
            .namespace_id = (registry_namespace_id_t[]) { _namespace_id }, \
            .schema_id = (registry_id_t[]) { _schema_id }, \
            .instance_id = (registry_id_t[]) { _instance_id }, \
            .path = NULL, \
            .path_len = 0, \
        }

#define _REGISTRY_PATH_4_AND_MORE(_namespace_id, _schema_id, _instance_id, ...) \
        (registry_path_t) { \
            .namespace_id = (registry_namespace_id_t[]) { _namespace_id }, \
            .schema_id = (registry_id_t[]) { _schema_id }, \
            .instance_id = (registry_id_t[]) { _instance_id }, \
            .path = (registry_id_t[]) { __VA_ARGS__ }, \
            .path_len = _REGISTRY_PATH_NUMARGS(__VA_ARGS__), \
        }

#define _REGISTRY_PATH_GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define REGISTRY_PATH(...) \
        _REGISTRY_PATH_GET_MACRO(_0, ## __VA_ARGS__, \
                                 _REGISTRY_PATH_4_AND_MORE, \
                                 _REGISTRY_PATH_4_AND_MORE, \
                                 _REGISTRY_PATH_4_AND_MORE, \
                                 _REGISTRY_PATH_4_AND_MORE, \
                                 _REGISTRY_PATH_4_AND_MORE, \
                                 _REGISTRY_PATH_4_AND_MORE, \
                                 _REGISTRY_PATH_4_AND_MORE, \
                                 _REGISTRY_PATH_3, \
                                 _REGISTRY_PATH_2, \
                                 _REGISTRY_PATH_1, \
                                 _REGISTRY_PATH_0) \
        (__VA_ARGS__)

#define REGISTRY_PATH_SYS(...) \
        REGISTRY_PATH(REGISTRY_NAMESPACE_SYS, ## __VA_ARGS__)

#define REGISTRY_PATH_APP(...) \
        REGISTRY_PATH(REGISTRY_NAMESPACE_APP, ## __VA_ARGS__)

/**
 * @brief Registers a new namespace for configuration schemas.
 *
 * @param[in] namespace Pointer to the namespace object.
 */
int registry_path_register_namespace(const registry_namespace_t *namespace);

/**
 * @brief Registers a new schema on a given namespace.
 *
 * @param[in] namespace_id ID of the namespace.
 * @param[in] schema Pointer to the schema structure.
 */
int registry_path_register_schema(const registry_id_t *namespace_id,
                                  const registry_schema_t *schema);

/**
 * @brief Sets the value of a parameter that belongs to a configuration group.
 *
 * @param[in] path Path of the parameter to be set
 * @param[in] val New value for the parameter
 * @return -EINVAL if schema could not be found, otherwise returns the
 *             value of the set schema function.
 */
int registry_set_value_by_path(const registry_path_t path, const registry_value_t val);

int registry_set_opaque_by_path(const registry_path_t path, const void *val,
                                const size_t val_len);
int registry_set_string_by_path(const registry_path_t path, const char *val);
int registry_set_bool_by_path(const registry_path_t path, const bool val);
int registry_set_uint8_by_path(const registry_path_t path, const uint8_t val);
int registry_set_uint16_by_path(const registry_path_t path, const uint16_t val);
int registry_set_uint32_by_path(const registry_path_t path, const uint32_t val);
#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64) || IS_ACTIVE(DOXYGEN)
int registry_set_uint64_by_path(const registry_path_t path, const uint64_t val);
#endif /* CONFIG_REGISTRY_USE_UINT64 */
int registry_set_int8_by_path(const registry_path_t path, const int8_t val);
int registry_set_int16_by_path(const registry_path_t path, const int16_t val);
int registry_set_int32_by_path(const registry_path_t path, const int32_t val);
#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64) || IS_ACTIVE(DOXYGEN)
int registry_set_int64_by_path(const registry_path_t path, const int64_t val);
#endif /* CONFIG_REGISTRY_USE_INT64 */
#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32) || IS_ACTIVE(DOXYGEN)
int registry_set_float32_by_path(const registry_path_t path, const float val);
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */
#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64) || IS_ACTIVE(DOXYGEN)
int registry_set_float64_by_path(const registry_path_t path, const double val);
#endif /* CONFIG_REGISTRY_USE_FLOAT64 */

/**
 * @brief Gets the current value of a parameter that belongs to a configuration
 *        group, identified by @p path.
 * @param[in] path Path of the parameter to get the value of
 * @param[out] value Pointer to a uninitialized @ref registry_value_t struct
 * @return 0 on success, non-zero on failure
 */
int registry_get_value_by_path(const registry_path_t path, registry_value_t *value);

int registry_get_opaque_by_path(const registry_path_t path, const void **buf,
                                size_t *buf_len);
int registry_get_string_by_path(const registry_path_t path, const char **buf,
                                size_t *buf_len);
int registry_get_bool_by_path(const registry_path_t path, const bool **buf);
int registry_get_uint8_by_path(const registry_path_t path, const uint8_t **buf);
int registry_get_uint16_by_path(const registry_path_t path, const uint16_t **buf);
int registry_get_uint32_by_path(const registry_path_t path, const uint32_t **buf);
#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64) || IS_ACTIVE(DOXYGEN)
int registry_get_uint64_by_path(const registry_path_t path, const uint64_t **buf);
#endif /* CONFIG_REGISTRY_USE_UINT64 */
int registry_get_int8_by_path(const registry_path_t path, const int8_t **buf);
int registry_get_int16_by_path(const registry_path_t path, const int16_t **buf);
int registry_get_int32_by_path(const registry_path_t path, const int32_t **buf);
#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64) || IS_ACTIVE(DOXYGEN)
int registry_get_int64_by_path(const registry_path_t path, const int64_t **buf);
#endif /* CONFIG_REGISTRY_USE_INT64 */
#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32) || IS_ACTIVE(DOXYGEN)
int registry_get_float32_by_path(const registry_path_t path, const float **buf);
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */
#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64) || IS_ACTIVE(DOXYGEN)
int registry_get_float64_by_path(const registry_path_t path, const double **buf);
#endif /* CONFIG_REGISTRY_USE_FLOAT64 */

/**
 * @brief If a @p path is passed it calls the commit schema for that
 *        configuration group. If no @p path is passed the commit schema is
 *        called for every registered configuration group.
 *
 * @param[in] path Path of the configuration group to commit the changes (can
 * be NULL).
 * @return 0 on success, -EINVAL if the group has not implemented the commit
 * function.
 */
int registry_commit_by_path(const registry_path_t path);

/**
 * @brief Load all configuration parameters that are included in the path from the registered storage.
 *
 * @param[in] path Path of the configuration parameters
 * @return 0 on success, non-zero on failure
 */
int registry_load_by_path(const registry_path_t path);

/**
 * @brief Save all configuration parameters of every configuration group to the
 * registered storage.
 *
 * @param[in] path Path of the configuration parameters
 * @return 0 on success, non-zero on failure
 */
int registry_save_by_path(const registry_path_t path);

typedef int registry_path_export_cb_t(const registry_path_t *path,
                                      const registry_export_data_t data,
                                      const registry_export_data_type_t data_type,
                                      const registry_value_t *value,
                                      const void *context);

/**
 * @brief Export an specific or all configuration parameters using the
 * @p export_func function. If @p path is NULL then @p export_func is called for
 * every configuration parameter on each configuration group.
 *
 * @param[in] export_func Exporting function call with the @p path and current
 * value of a specific or all configuration parameters
 * @param[in] path Path representing the configuration parameter. Can be NULL.
 * @param[in] recursion_depth Defines how deeply nested child groups / parameters will be shown. (0 to show all children, 1 to only show the exact match, 2 - n to show the exact match plus its children ... plus n levels of children )
 * @param[in] context Context that will be passed to @p export_func
 * @return 0 on success, non-zero on failure
 */
int registry_export_by_path(const registry_path_export_cb_t *export_cb, const registry_path_t *path,
                            const int recursion_depth, const void *context);



#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_PATH_H */
/** @} */
