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
 * @author      Leandro Lanzieri <leandro.lanzieri@haw-hamburg.de>
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_H
#define REGISTRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "kernel_defines.h"
#include "clist.h"

/**
 * @brief Maximum amount of levels of hierarchy in configurations names.
 */
#define REGISTRY_MAX_DIR_DEPTH     8

/**
 * @brief Maximum amount of characters per level in configurations names.
 */
#define REGISTRY_MAX_DIR_NAME_LEN  10 /* a path is an uint32_t and uint32_t MAX has 10 digits. */

/**
 * @brief Maximum length of a configuration name.
 * @{
 */
#define REGISTRY_MAX_DIR_LEN      ((REGISTRY_MAX_DIR_NAME_LEN * \
                                    REGISTRY_MAX_DIR_DEPTH) + \
                                   (REGISTRY_MAX_DIR_DEPTH - 1))
/** @} */

/**
 * @brief Calculates the size of an @ref registry_schema_item_t array.
 *
 */
#define _REGISTRY_SCHEMA_ITEM_NUMARGS(...)  (sizeof((registry_schema_item_t[]){ __VA_ARGS__ }) / \
                                             sizeof(registry_schema_item_t))

/**
 * @brief Creates and initializes a @ref registry_schema_t struct.
 *
 */
#define REGISTRY_SCHEMA(_field_name, _id, _name, _description, _mapping, ...) \
    registry_schema_t _field_name = { \
        .id = _id, \
        .name = _name, \
        .description = _description, \
        .mapping = _mapping, \
        .items = (registry_schema_item_t[]) { __VA_ARGS__ }, \
        .items_len = _REGISTRY_SCHEMA_ITEM_NUMARGS(__VA_ARGS__), \
    }

/**
 * @brief Creates and initializes a @ref registry_schema_item_t struct and defaults its type to @ref REGISTRY_SCHEMA_TYPE_GROUP.
 *
 */
#define REGISTRY_GROUP(_id, _name, _description, ...) \
    { \
        .id = _id, \
        .name = _name, \
        .description = _description, \
        .type = REGISTRY_SCHEMA_TYPE_GROUP, \
        .value.group = { \
            .items = (registry_schema_item_t[]) { __VA_ARGS__ }, \
            .items_len = _REGISTRY_SCHEMA_ITEM_NUMARGS(__VA_ARGS__), \
        }, \
    },

/**
 * @brief Creates and initializes a @ref registry_schema_item_t struct and defaults its type to @ref REGISTRY_SCHEMA_TYPE_PARAMETER.
 *
 */
#define REGISTRY_PARAMETER(_id, _name, _description, _type) \
    { \
        .id = _id, \
        .name = _name, \
        .description = _description, \
        .type = REGISTRY_SCHEMA_TYPE_PARAMETER, \
        .value.parameter = { \
            .type = _type, \
        }, \
    },

#if IS_ACTIVE(CONFIG_REGISTRY_DISABLE_SCHEMA_NAME_FIELD) && \
    IS_ACTIVE(CONFIG_REGISTRY_DISABLE_SCHEMA_DESCRIPTION_FIELD)
/* no name and no description */
# define _REGISTRY_PARAMETER(_id, _name, _description, _type) \
    REGISTRY_PARAMETER(_id, "", "", _type)
#elif IS_ACTIVE(CONFIG_REGISTRY_DISABLE_SCHEMA_NAME_FIELD)
/* no name */
# define _REGISTRY_PARAMETER(_id, _name, _description, _type) \
    REGISTRY_PARAMETER(_id, "", _description, _type)
#elif IS_ACTIVE(CONFIG_REGISTRY_DISABLE_SCHEMA_DESCRIPTION_FIELD)
/* no description */
# define _REGISTRY_PARAMETER(_id, _name, _description, _type) \
    REGISTRY_PARAMETER(_id, _name, "", _type)
#else
/* keep name and description */
# define _REGISTRY_PARAMETER(_id, _name, _description, _type) \
    REGISTRY_PARAMETER(_id, _name, _description, _type)
#endif

#define REGISTRY_PARAMETER_STRING(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_STRING)
#define REGISTRY_PARAMETER_BOOL(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_BOOL)
#define REGISTRY_PARAMETER_UINT8(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_UINT8)
#define REGISTRY_PARAMETER_UINT16(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_UINT16)
#define REGISTRY_PARAMETER_UINT32(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_UINT32)

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64) || IS_ACTIVE(DOXYGEN)
# define REGISTRY_PARAMETER_UINT64(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_UINT64)
#else
# define REGISTRY_PARAMETER_UINT64(_id, _name, _description)
#endif /* CONFIG_REGISTRY_USE_UINT64 */

#define REGISTRY_PARAMETER_INT8(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_INT8)
#define REGISTRY_PARAMETER_INT16(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_INT16)
#define REGISTRY_PARAMETER_INT32(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_INT32)

#if IS_ACTIVE(CONFIG_REGISTRY_USE_INT64) || IS_ACTIVE(DOXYGEN)
# define REGISTRY_PARAMETER_INT64(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_INT64)
#else
# define REGISTRY_PARAMETER_INT64(_id, _name, _description)
#endif /* CONFIG_REGISTRY_USE_INT64 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT32) || IS_ACTIVE(DOXYGEN)
# define REGISTRY_PARAMETER_FLOAT32(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_FLOAT32)
#else
# define REGISTRY_PARAMETER_FLOAT32(_id, _name, _description)
#endif /* CONFIG_REGISTRY_USE_FLOAT32 */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_FLOAT64) || IS_ACTIVE(DOXYGEN)
# define REGISTRY_PARAMETER_FLOAT64(_id, _name, _description) \
    _REGISTRY_PARAMETER(_id, _name, _description, REGISTRY_TYPE_FLOAT64)
#else
# define REGISTRY_PARAMETER_FLOAT64(_id, _name, _description)
#endif /* CONFIG_REGISTRY_USE_FLOAT64 */

/**
 * @brief Data types of the registry
 *
 * @note Float and int64 types must be enabled by defining
 * `CONFIG_REGISTRY_USE_FLOAT` and `CONFIG_REGISTRY_USE_INT64`. Use with caution
 * as they bloat the code size.
 */
typedef enum {
    REGISTRY_TYPE_NONE = 0,     /**< No type specified */
    REGISTRY_TYPE_OPAQUE,       /**< OPAQUE */
    REGISTRY_TYPE_STRING,       /**< String */
    REGISTRY_TYPE_BOOL,         /**< Boolean */

    REGISTRY_TYPE_UINT8,        /**< 8-bits unsigned integer */
    REGISTRY_TYPE_UINT16,       /**< 16-bits unsigned integer */
    REGISTRY_TYPE_UINT32,       /**< 32-bits unsigned integer */

#if IS_ACTIVE(CONFIG_REGISTRY_USE_UINT64) || IS_ACTIVE(DOXYGEN)
    REGISTRY_TYPE_UINT64,     /**< 64-bits unsigned integer */
#endif /* CONFIG_REGISTRY_USE_UINT64 */

    REGISTRY_TYPE_INT8,         /**< 8-bits signed integer */
    REGISTRY_TYPE_INT16,        /**< 16-bits signed integer */
    REGISTRY_TYPE_INT32,        /**< 32-bits signed integer */

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


typedef enum {
    REGISTRY_NAMESPACE_SYS,
    REGISTRY_NAMESPACE_APP,
} registry_namespace_id_t;

typedef struct {
    registry_namespace_id_t id;     /**< Integer representing the configuration namespace */
    char *name;                     /**< String describing the configuration namespace */
    char *description;              /**< String describing the configuration namespace with more details */
    clist_node_t schemas;           /**< Linked list of schemas @ref registry_schema_t */
} registry_namespace_t;

extern registry_namespace_t registry_namespace_sys;
extern registry_namespace_t registry_namespace_app;

typedef uint32_t registry_id_t;

/**
 * @brief Basic representation of a registry parameter, containing information about its type and its value.
 */
typedef struct {
    registry_type_t type;   /**< The type of the parameter */
    const void *buf;        /**< Pointer to the buffer containing the value of the parameter */
    size_t buf_len;         /**< Length of the buffer */
} registry_value_t;

/**
 * @brief Parameter of a configuration group.
 */
typedef struct {
    const registry_type_t type; /**< Enum representing the type of the configuration parameter */
} registry_schema_parameter_t;

typedef struct _registry_schema_item_t registry_schema_item_t;

/**
 * @brief Configuration group.
 */
typedef struct {
    const registry_schema_item_t *items;
    const size_t items_len;
} registry_schema_group_t;

typedef enum {
    REGISTRY_SCHEMA_TYPE_GROUP,
    REGISTRY_SCHEMA_TYPE_PARAMETER,
} registry_schema_type_t;

struct _registry_schema_item_t {
    const registry_id_t id;                             /**< Integer representing the path id of the schema item */
    const char *name;                                   /**< String describing the schema item */
    const char *description;                            /**< String describing the schema item with more details */
    const registry_schema_type_t type;                  /**< Type of the schema item (group or parameter) */
    const union {
        const registry_schema_group_t group;            /**< Value of the schema item if it is a group. Contains an array of schema item children */
        const registry_schema_parameter_t parameter;    /**< Value of the schema item if it is a parameter. Contains its type */
    } value;                                            /**< Union containing either group or parameter data */
};

/**
 * @brief Instance of a schema containing its data.
 */
typedef struct {
    clist_node_t node;  /**< Linked list node */
    char *name;         /**< String describing the instance */
    void *data;         /**< Struct containing all configuration parameters of the schema */

    /**
     * @brief Will be called after @ref registry_commit() was called on this instance.
     *
     * @param[in] id ID of the group or parameter to commit changes to, commits the whole instance on NULL
     * @param[in] context Context of the instance
     * @return 0 on success, non-zero on failure
     */
    int (*commit_cb)(const registry_id_t *id, const void *context);

    void *context; /**< Optional context used by the instance */
} registry_instance_t;

/**
 * @brief Schema for configuration groups. Each configuration group should
 * register a schema using the @ref registry_register_schema() function.
 * A schema provides the pointer to get, set and commit configuration
 * parameters.
 */
typedef struct {
    clist_node_t node;                      /**< Linked list node */
    const registry_id_t id;                 /**< Integer representing the configuration group */
    const char *name;                       /**< String describing the configuration group */
    const char *description;                /**< String describing the configuration group with more details */
    const registry_schema_item_t *items;    /**< Array representing all the configuration parameters that belong to this group */
    const size_t items_len;                 /**< Size of items array */
    clist_node_t instances;                 /**< Linked list of schema instances @ref registry_instance_t */

    /**
     * @brief Mapping to connect configuration parameter IDs with the address in the storage.
     *
     * @param[in] param_id ID of the parameter that contains the value
     * @param[in] instance Pointer to the instance of the schema, that contains the parameter
     * @param[in] val Pointer to buffer containing the new value
     * @param[in] val_len Pointer to length of the buffer to store the current value
     */
    void (*mapping)(const registry_id_t param_id, const registry_instance_t *instance, void **val,
                    size_t *val_len);
} registry_schema_t;

/**
 * @brief Registers a new sys schema for a configuration group.
 *
 * @param[in] namespace_id ID of the namespace.
 * @param[in] schema Pointer to the schema structure.
 */
int registry_register_schema(const registry_namespace_id_t namespace_id,
                             const registry_schema_t *schema);

/**
 * @brief Adds a new instance of a schema.
 *
 * @param[in] namespace_id ID of the namespace.
 * @param[in] schema_id ID of the schema.
 * @param[in] instance Pointer to instance structure.
 */
int registry_register_schema_instance(const registry_namespace_id_t namespace_id,
                                      const registry_id_t schema_id,
                                      const registry_instance_t *instance);














/* ----------------------------- WIP ----------------------------- */

/**
 * Remove parenthesises
 */
#define _REMOVE_PARENTHESISES_INTERN(...) __VA_ARGS__
#define _REMOVE_PARENTHESISES(...) _REMOVE_PARENTHESISES_INTERN __VA_ARGS__


/**
 * IF inside macros
 */
#define CONCAT2(A, B) A ## B
#define CONCAT2_DEFERRED(A, B) CONCAT2(A, B)
#define IF_0(true_case, false_case) false_case
#define IF_1(true_case, false_case) true_case
#define IF(condition, true_case, false_case) \
    CONCAT2_DEFERRED(IF_, condition)(true_case, false_case)


/**
 * Accept any number of args >= N, but expand to just the Nth one.
 * Here, N == 6.
 */
#define _REGISTRY_GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N

/**
 * Define some macros to help us create overrides based on the
 * Here, N == 6.
 */
#define _registry_fe_0(_call, ...)
#define _registry_fe_1(_call, x) _call x
#define _registry_fe_2(_call, x, ...) _call x _registry_fe_1(_call, __VA_ARGS__)
#define _registry_fe_3(_call, x, ...) _call x _registry_fe_2(_call, __VA_ARGS__)
#define _registry_fe_4(_call, x, ...) _call x _registry_fe_3(_call, __VA_ARGS__)

/**
 * Provide a for-each construct for variadic macros. Supports up
 * to 4 args.
 */
#define _CALL_MACRO_FOR_EACH(x, ...) \
    _REGISTRY_GET_NTH_ARG( \
        "ignored", ## __VA_ARGS__, \
        _registry_fe_4, _registry_fe_3, _registry_fe_2, _registry_fe_1, _registry_fe_0 \
        ) (x, __VA_ARGS__)



int registry_get_uint8_v2(uint8_t **val, size_t *val_len);

#define _REGISTRY_SCHEMA_ITEM_NUMARGS_V2(...)  (sizeof((registry_schema_item_t[]){ __VA_ARGS__ }) / \
                                                sizeof(registry_schema_item_t))

/* declaration */
#define _REGISTRY_SCHEMA_ITEM_DECLARATION_V2(_is_group, ...) \
    _REMOVE_PARENTHESISES( \
        IF( \
            _is_group, \
            (_REGISTRY_SCHEMA_GROUP_DECLARATION_V2(__VA_ARGS__)), \
            (_REGISTRY_SCHEMA_PARAMETER_DECLARATION_V2(__VA_ARGS__)) \
            ))


#define _REGISTRY_SCHEMA_PARAMETER_DECLARATION_V2(_field_name, _type) \
    struct { \
        int (*get)(_type **val, size_t *val_len); \
    } _field_name;

#define _REGISTRY_SCHEMA_GROUP_DECLARATION_V2(_field_name, ...) \
    struct { \
        _CALL_MACRO_FOR_EACH(_REGISTRY_SCHEMA_ITEM_DECLARATION_V2, __VA_ARGS__) \
    } _field_name;

/* initialization */
#define _REGISTRY_SCHEMA_ITEM_INITIALIZATION_V2(_is_group, ...) \
    _REMOVE_PARENTHESISES( \
        IF( \
            _is_group, \
            (_REGISTRY_SCHEMA_GROUP_INITIALIZATION_V2(__VA_ARGS__)), \
            (_REGISTRY_SCHEMA_PARAMETER_INITIALIZATION_V2(__VA_ARGS__)) \
            ))

#define _REGISTRY_SCHEMA_PARAMETER_INITIALIZATION_V2(_field_name, _type) \
    ._field_name = { \
        .get = registry_get_uint8_v2, \
    },

#define _REGISTRY_SCHEMA_GROUP_INITIALIZATION_V2(_field_name, ...) \
    ._field_name = { \
        _CALL_MACRO_FOR_EACH(_REGISTRY_SCHEMA_ITEM_INITIALIZATION_V2, __VA_ARGS__) \
    },

/* registry schema macros */
#define REGISTRY_SCHEMA_V2(_field_name, _id, ...) \
    typedef struct { \
        struct { \
            registry_id_t id; \
        } meta; \
        _CALL_MACRO_FOR_EACH(_REGISTRY_SCHEMA_ITEM_DECLARATION_V2, __VA_ARGS__) \
    } registry_schema_ ## _field_name ## _t; \
    \
    registry_schema_ ## _field_name ## _t registry_schema_ ## _field_name = { \
        .meta = { \
            .id = _id, \
        }, \
        _CALL_MACRO_FOR_EACH(_REGISTRY_SCHEMA_ITEM_INITIALIZATION_V2, __VA_ARGS__) \
    };

#define REGISTRY_PARAMETER_V2(_field_name, _type) \
    (0, _field_name, _type)

#define REGISTRY_GROUP_V2(_field_name, ...) \
    (1, _field_name, __VA_ARGS__)




#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_H */
/** @} */
