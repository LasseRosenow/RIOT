/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry RIOT Registry
 * @ingroup     sys
 * @brief       RIOT Registry module for handling runtime configurations
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_H
#define REGISTRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "clist.h"
#include "modules.h"

// TODO REMOVE THIS START
#define CONFIG_REGISTRY_ENABLE_META_DESCRIPTION 1
#define CONFIG_REGISTRY_ENABLE_META_NAME 1
#define MODULE_REGISTRY_PATH 1
#define CONFIG_REGISTRY_USE_UINT64 1
#define CONFIG_REGISTRY_USE_INT64 1
#define CONFIG_REGISTRY_USE_FLOAT32 1
#define CONFIG_REGISTRY_USE_FLOAT64 1
// TODO REMOVE THIS END

typedef uint8_t registry_namespace_id_t;
typedef uint32_t registry_schema_id_t;
typedef uint16_t registry_instance_id_t;
typedef uint16_t registry_resource_id_t;

typedef struct _registry_namespace_t registry_namespace_t;
typedef struct _registry_schema_t registry_schema_t;
typedef struct _registry_instance_t registry_instance_t;
typedef struct _registry_resource_t registry_resource_t;

/**
 * @brief Data types of the registry
 *
 * @note Float and int64 types must be enabled by defining
 * `CONFIG_REGISTRY_USE_FLOAT` and `CONFIG_REGISTRY_USE_INT64`. Use with caution
 * as they bloat the code size.
 */
typedef enum {
    REGISTRY_TYPE_NONE  = 0,        /**< No type specified */
    REGISTRY_TYPE_GROUP = 1,        /**< GROUP contains parameters */

    REGISTRY_TYPE_OPAQUE,           /**< OPAQUE */
    REGISTRY_TYPE_STRING,           /**< String */
    REGISTRY_TYPE_BOOL,             /**< Boolean */

    REGISTRY_TYPE_UINT8,            /**< 8-bits unsigned integer */
    REGISTRY_TYPE_UINT16,           /**< 16-bits unsigned integer */
    REGISTRY_TYPE_UINT32,           /**< 32-bits unsigned integer */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64) || IS_ACTIVE(DOXYGEN)
    REGISTRY_TYPE_UINT64,     /**< 64-bits unsigned integer */
#endif /* CONFIG_REGISTRY_USE_UINT64 */

    REGISTRY_TYPE_INT8,             /**< 8-bits signed integer */
    REGISTRY_TYPE_INT16,            /**< 16-bits signed integer */
    REGISTRY_TYPE_INT32,            /**< 32-bits signed integer */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64) || IS_ACTIVE(DOXYGEN)
    REGISTRY_TYPE_INT64,     /**< 64-bits signed integer */
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32) || IS_ACTIVE(DOXYGEN)
    REGISTRY_TYPE_FLOAT32,     /**< 32-bits float */
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64) || IS_ACTIVE(DOXYGEN)
    REGISTRY_TYPE_FLOAT64,     /**< 64-bits float */
#endif /* CONFIG_REGISTRY_USE_FLOAT64 */
} registry_type_t;

/**
 * @brief Basic representation of a configuration parameter value.
 */
typedef struct {
    registry_type_t type;   /**< The type of the configuration parameter value */
    const void *buf;        /**< Pointer to the buffer containing the value of the configuration parameter */
    size_t buf_len;         /**< Length of the buffer */
} registry_value_t;

typedef const enum {
    REGISTRY_COMMIT_INSTANCE,
    REGISTRY_COMMIT_GROUP,
    REGISTRY_COMMIT_PARAMETER,
} registry_commit_cb_scope_t;

/**
 * @brief Instance of a schema containing its data.
 */
struct _registry_instance_t {
    clist_node_t node;                  /**< Linked list node */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_META_NAME) || IS_ACTIVE(DOXYGEN)
    const char * const name;            /**< String describing the instance */
#endif /* CONFIG_REGISTRY_ENABLE_META_NAME */
    const void * const data;            /**< Struct containing all configuration parameters of the schema */
    const registry_schema_t *schema;    /**< Configuration Schema that the Schema Instance belongs to */

    /**
     * @brief Will be called after @ref registry_commit() was called on this instance.
     *
     * @param[in] scope Scope of what will be committed (a parameter, a group or the whole instance)
     * @param[in] id ID of the group or parameter to commit changes to, commits the whole instance on NULL
     * @param[in] context Context of the instance
     * @return 0 on success, non-zero on failure
     */
    int (*commit_cb)(const registry_commit_cb_scope_t scope,
                     const registry_resource_id_t *resource_id,
                     const void *context);

    void *context; /**< Optional context used by the instance */
};

struct _registry_resource_t {
#if IS_USED(MODULE_REGISTRY_PATH) || IS_ACTIVE(DOXYGEN)
    const registry_resource_id_t id;                         /**< Integer representing the path id of the configuration resource */
#endif /* MODULE_REGISTRY_PATH */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_META_NAME) || IS_ACTIVE(DOXYGEN)
    const char * const name;                        /**< String describing the configuration resource */
#endif /* CONFIG_REGISTRY_ENABLE_META_NAME */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_META_DESCRIPTION) || IS_ACTIVE(DOXYGEN)
    const char * const description;                 /**< String describing the configuration resource with more details */
#endif /* CONFIG_REGISTRY_ENABLE_META_DESCRIPTION */
    const registry_schema_t * const schema;         /**< Configuration Schema that the configuration resource belongs to */
    const registry_type_t type;                     /**< Type of the configuration resource (group or parameter) */
    const registry_resource_t ** const resources;   /**< Array of pointers to all the configuration parameters and groups that belong to this group */
    const size_t resources_len;                     /**< Size of resources array */
};

/**
 * @brief Schema containing available configuration parameters.
 */
struct _registry_schema_t {
#if IS_USED(MODULE_REGISTRY_PATH) || IS_ACTIVE(DOXYGEN)
    const registry_schema_id_t id;                         /**< Integer representing the path id of the schema */
#endif /* MODULE_REGISTRY_PATH */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_META_NAME) || IS_ACTIVE(DOXYGEN)
    const char * const name;                        /**< String describing the schema */
#endif /* CONFIG_REGISTRY_ENABLE_META_NAME */
#if IS_ACTIVE(CONFIG_REGISTRY_ENABLE_META_DESCRIPTION) || IS_ACTIVE(DOXYGEN)
    const char * const description;                 /**< String describing the schema with more details */
#endif /* CONFIG_REGISTRY_ENABLE_META_DESCRIPTION */
    const registry_namespace_t * const namespace;   /**< Configuration Namespace that the Configuration Schema belongs to */
    clist_node_t instances;                         /**< Linked list of schema instances @ref registry_instance_t */
    const registry_resource_t ** const resources;   /**< Array of pointers to all the configuration parameters and groups that belong to this schema */
    const size_t resources_len;                     /**< Size of resources array */

    /**
     * @brief Mapping to connect configuration parameter IDs with the address in the storage.
     *
     * @param[in] parameter_id ID of the parameter that contains the value
     * @param[in] instance Pointer to the instance of the schema, that contains the parameter
     * @param[in] val Pointer to buffer containing the new value
     * @param[in] val_len Pointer to length of the buffer to store the current value
     */
    void(*const mapping)(const registry_resource_id_t parameter_id,
                         const registry_instance_t *instance,
                         void **val,
                         size_t *val_len);
};

struct _registry_namespace_t {
    clist_node_t node;                          /**< Linked list node */
    const char * const name;                    /**< String describing the configuration namespace */
    const char * const description;             /**< String describing the configuration namespace with more details */
    const registry_schema_t ** const schemas;   /**< Array of pointers to all the configuration schemas that belong to this namespace */
    const size_t schemas_len;                   /**< Size of schemas array */
};

extern clist_node_t _registry_namespaces;

/**
 * @brief Adds a new namespace to the registry.
 *
 * @param[in] namespace Pointer to the new namespace.
 */
int registry_register_namespace(const registry_namespace_t *namespace);

/**
 * @brief Adds a new instance to a schema.
 *
 * @param[in] schema Pointer to the schema.
 * @param[in] instance Pointer to the new instance.
 */
int registry_register_schema_instance(const registry_schema_t *schema,
                                      const registry_instance_t *instance);

/**
 * @brief Gets the current value of a parameter that belongs to an instance
 * of a configuration schema.
 *
 * @param[in] instance Pointer to the configuration schema instance.
 * @param[in] parameter Pointer to the configuration parameter.
 * @param[out] value Pointer to a uninitialized @ref registry_value_t struct.
 * @return 0 on success, non-zero on failure
 */
int registry_get(const registry_instance_t *instance, const registry_resource_t *parameter,
                 registry_value_t *value);

/**
 * @brief Sets the value of a configuration parameter that belongs to an instance
 * of a configuration schema.
 *
 * @param[in] instance Pointer to the configuration schema instance.
 * @param[in] parameter Pointer to the configuration parameter.
 * @param[in] value Pointer to the new value for the configuration parameter.
 * @return 0 on success, non-zero on failure
 */
int registry_set(const registry_instance_t *instance, const registry_resource_t *parameter,
                 const registry_value_t *value);

/**
 * @brief Commits every configuration parameter.
 */
int registry_commit(void);

/**
 * @brief Commits every configuration parameter within the given configuration namespace.
 *
 * @param[in] namespace Pointer to the configuration namespace.
 */
int registry_commit_namespace(const registry_namespace_t *namespace);

/**
 * @brief Commits every configuration parameter within the given configuration schema.
 *
 * @param[in] schema Pointer to the configuration schema.
 */
int registry_commit_schema(const registry_schema_t *schema);

/**
 * @brief Commits every configuration parameter within the given configuration schema instance.
 *
 * @param[in] instance Pointer to the configuration schema instance.
 */
int registry_commit_instance(const registry_instance_t *instance);

/**
 * @brief Commits every configuration parameter within the given configuration group.
 *
 * @param[in] instance Pointer to the configuration schema instance of the configuration group.
 * @param[in] group Pointer to the configuration group.
 */
int registry_commit_group(const registry_instance_t *instance, const registry_resource_t *group);

/**
 * @brief Commits the given configuration parameter.
 *
 * @param[in] instance Pointer to the configuration schema instance of the configuration parameter.
 * @param[in] parameter Pointer to the configuration parameter.
 */
int registry_commit_parameter(const registry_instance_t *instance,
                              const registry_resource_t *parameter);

typedef const union {
    const registry_namespace_t *namespace;
    const registry_schema_t *schema;
    const registry_instance_t *instance;
    const registry_resource_t *group;
    const struct {
        const registry_resource_t *data;
        const registry_value_t *value;
        const registry_instance_t *instance;
    } parameter;
} registry_export_cb_data_t;

typedef const enum {
    REGISTRY_EXPORT_INSTANCE,
    REGISTRY_EXPORT_NAMESPACE,
    REGISTRY_EXPORT_SCHEMA,
    REGISTRY_EXPORT_GROUP,
    REGISTRY_EXPORT_PARAMETER,
} registry_export_cb_data_type_t;

typedef int (*registry_export_cb_t)(const registry_export_cb_data_t *data,
                                    const registry_export_cb_data_type_t data_type,
                                    const void *context);

#define REGISTRY_EXPORT_ALL = 0;
#define REGISTRY_EXPORT_SELF = 1;
#define REGISTRY_EXPORT_WITH_N_LEVELS_OF_CHILDREN(_n) (_n + 1);

/**
 * @brief Exports every configuration parameter.
 *
 * @param[in] export_cb Exporting callback function call with the @p path and current
 * value of a specific or all configuration parameters
 * @param[in] recursion_depth Defines how deeply nested child groups / parameters
 * will be shown. (0 to show all children, 1 to only show the exact match, n > 1
 * to show the exact match plus n - 1 levels of children)
 * @param[in] context Context that will be passed to @p export_cb
 */
int registry_export(const registry_export_cb_t export_cb,
                    const uint8_t recursion_depth, const void *context);

/**
 * @brief Exports every configuration parameter within the given configuration namespace.
 *
 * @param[in] namespace Pointer to the configuration namespace.
 * @param[in] export_cb Exporting callback function call with the @p path and current
 * value of a specific or all configuration parameters
 * @param[in] recursion_depth Defines how deeply nested child groups / parameters
 * will be shown. (0 to show all children, 1 to only show the exact match, 2 - n
 * to show the exact match plus its children ... plus n levels of children)
 * @param[in] context Context that will be passed to @p export_cb
 */
int registry_export_namespace(const registry_namespace_t *namespace,
                              const registry_export_cb_t export_cb, const uint8_t recursion_depth,
                              const void *context);

/**
 * @brief Exports every configuration parameter within the given configuration schema.
 *
 * @param[in] schema Pointer to the configuration schema.
 * @param[in] export_cb Exporting callback function call with the @p path and current
 * value of a specific or all configuration parameters
 * @param[in] recursion_depth Defines how deeply nested child groups / parameters
 * will be shown. (0 to show all children, 1 to only show the exact match, 2 - n
 * to show the exact match plus its children ... plus n levels of children)
 * @param[in] context Context that will be passed to @p export_cb
 */
int registry_export_schema(const registry_schema_t *schema, const registry_export_cb_t export_cb,
                           const uint8_t recursion_depth, const void *context);

/**
 * @brief Exports every configuration parameter within the given configuration schema instance.
 *
 * @param[in] instance Pointer to the configuration schema instance.
 * @param[in] export_cb Exporting callback function call with the @p path and current
 * value of a specific or all configuration parameters
 * @param[in] recursion_depth Defines how deeply nested child groups / parameters
 * will be shown. (0 to show all children, 1 to only show the exact match, 2 - n
 * to show the exact match plus its children ... plus n levels of children)
 * @param[in] context Context that will be passed to @p export_cb
 */
int registry_export_instance(const registry_instance_t *instance,
                             const registry_export_cb_t export_cb, const uint8_t recursion_depth,
                             const void *context);

/**
 * @brief Exports every configuration parameter within the given configuration group.
 *
 * @param[in] instance Pointer to the configuration schema instance.
 * @param[in] group Pointer to the configuration group.
 * @param[in] export_cb Exporting callback function call with the @p path and current
 * value of a specific or all configuration parameters
 * @param[in] recursion_depth Defines how deeply nested child groups / parameters
 * will be shown. (0 to show all children, 1 to only show the exact match, 2 - n
 * to show the exact match plus its children ... plus n levels of children)
 * @param[in] context Context that will be passed to @p export_cb
 */
int registry_export_group(const registry_instance_t *instance, const registry_resource_t *group,
                          const registry_export_cb_t export_cb, const uint8_t recursion_depth,
                          const void *context);

/**
 * @brief Exports the given configuration parameter.
 *
 * @param[in] instance Pointer to the configuration schema instance.
 * @param[in] parameter Pointer to the configuration parameter.
 * @param[in] export_cb Exporting callback function call with the @p path and current
 * value of a specific or all configuration parameters
 * @param[in] context Context that will be passed to @p export_cb
 */
int registry_export_parameter(const registry_instance_t *instance,
                              const registry_resource_t *parameter,
                              const registry_export_cb_t export_cb, const void *context);

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_H */
/** @} */
