/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_storage RIOT Registry Storage
 * @ingroup     sys
 * @brief       RIOT Registry Storage module allowing to store configuration parameters to non-volatile storage
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_STORAGE_H
#define REGISTRY_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"

typedef struct {
    registry_namespace_id_t *namespace_id;
    registry_id_t *schema_id;
    registry_id_t *instance_id;
    registry_id_t *path;
    size_t path_len;
} storage_path_t;

/**
 * @brief Prototype of a callback function for the load action of a storage facility
 * interface
 */
typedef void (*load_cb_t)(const storage_path_t path, const registry_value_t val,
                          const void *cb_arg);

typedef struct _registry_storage_facility_t registry_storage_facility_t;

/**
 * @brief Store facility descriptor
 */
typedef struct {
    clist_node_t node;                  /**< linked list node */
    registry_storage_facility_t *itf;   /**< interface for the facility */
    void *data;                         /**< Struct containing all config data for the storage facility */
} registry_storage_facility_instance_t;

/**
 * @brief Storage facility interface.
 * All storage facilities should, at least, implement the load and save actions.
 */
struct _registry_storage_facility_t {
    /**
     * @brief Loads all saved parameters and calls the @p cb callback function.
     *
     * @param[in] instance Storage facility descriptor
     * @param[in] path Path of the parameter
     * @param[in] cb Callback function to call for every saved parameter
     * @param[in] cb_arg Argument passed to @p cb function
     * @return 0 on success, non-zero on failure
     */
    int (*load)(const registry_storage_facility_instance_t *instance, const storage_path_t path,
                const load_cb_t cb, const void *cb_arg);

    /**
     * @brief If implemented, it is used for any preparation the storage may
     * need before starting a saving process.
     *
     * @param[in] instance Storage facility descriptor
     * @return 0 on success, non-zero on failure
     */
    int (*save_start)(const registry_storage_facility_instance_t *instance);

    /**
     * @brief Saves a parameter into storage.
     *
     * @param[in] instance Storage facility descriptor
     * @param[in] path Path of the parameter
     * @param[in] value Struct representing the value of the parameter
     * @return 0 on success, non-zero on failure
     */
    int (*save)(const registry_storage_facility_instance_t *instance, const storage_path_t path,
                const registry_value_t value);

    /**
     * @brief If implemented, it is used for any tear-down the storage may need
     * after a saving process.
     *
     * @param[in] instance Storage facility descriptor
     * @return 0 on success, non-zero on failure
     */
    int (*save_end)(const registry_storage_facility_instance_t *instance);
};

extern const registry_storage_facility_instance_t *_storage_facility_dst;
extern clist_node_t _storage_facility_srcs;

/**
 * @brief Registers a new storage as a source of configurations. Multiple
 *        storages can be configured as sources at the same time. Configurations
 *        will be loaded from all of them. This is commonly called by the
 *        storage facilities who implement their own registry_<storage-name>_src
 *        function.
 *
 * @param[in] src Pointer to the storage to register as source.
 */
void registry_register_storage_facility_src(const registry_storage_facility_instance_t *src);

/**
 * @brief Registers a new storage as a destination for saving configurations.
 *        Only one storage can be registered as destination at a time. If a
 *        previous storage had been registered before it will be replaced by the
 *        new one. This is commonly called by the storage facilities who
 *        implement their own registry_<storage-name>_dst function.
 *
 * @param[in] dst Pointer to the storage to register
 */
void registry_register_storage_facility_dst(const registry_storage_facility_instance_t *dst);

/* heap */
#if IS_USED(MODULE_REGISTRY_STORAGE_HEAP) || IS_ACTIVE(DOXYGEN)
extern registry_storage_facility_t registry_storage_facility_heap;
#endif

/* vfs */
#if IS_USED(MODULE_REGISTRY_STORAGE_VFS) || IS_ACTIVE(DOXYGEN)
extern registry_storage_facility_t registry_storage_facility_vfs;
#endif

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_STORAGE_H */
/** @} */
