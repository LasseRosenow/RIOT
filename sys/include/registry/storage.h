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

/**
 * @brief Prototype of a callback function for the load action of a storage interface.
 */
typedef int (*load_cb_t)(const registry_instance_t *instance, const registry_parameter_t *parameter,
                         const void *buf, const size_t buf_len);

typedef struct _registry_storage_t registry_storage_t;

/**
 * @brief Storage descriptor.
 */
typedef struct {
    registry_storage_t *storage;        /**< interface of the storage. */
    void *data;                         /**< Struct containing all config data for the storage. */
} registry_storage_instance_t;

/**
 * @brief Storage interface. All storages should, at least, implement the load and save actions.
 */
struct _registry_storage_t {
    /**
     * @brief Loads all saved parameters and calls the @p load_cb callback function.
     *
     * @param[in] storage Storage descriptor.
     * @param[in] load_cb Callback function to call for every saved parameter.
     *
     * @return 0 on success, non-zero on failure.
     */
    int (*load)(const registry_storage_instance_t *storage,
                const load_cb_t load_cb);

    /**
     * @brief If implemented, it is used for any preparation the storage may
     * need before starting a saving process.
     *
     * @param[in] storage Storage descriptor.
     *
     * @return 0 on success, non-zero on failure.
     */
    int (*save_start)(const registry_storage_instance_t *storage);

    /**
     * @brief Saves a parameter into storage.
     *
     * @param[in] storage Storage descriptor.
     * @param[in] instance Pointer to the configuration schema instance.
     * @param[in] parameter Pointer to the configuration parameter.
     * @param[in] value Configuration parameter value.
     *
     * @return 0 on success, non-zero on failure.
     */
    int (*save)(const registry_storage_instance_t *storage,
                const registry_instance_t *instance,
                const registry_parameter_t *parameter,
                const registry_value_t *value);

    /**
     * @brief If implemented, it is used for any tear-down the storage may need
     * after a saving process.
     *
     * @param[in] storage Storage descriptor.
     *
     * @return 0 on success, non-zero on failure.
     */
    int (*save_end)(const registry_storage_instance_t *storage);
};

/**
 * @brief Load all configuration parameters from the registered storage.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_load(void);

/**
 * @brief Save all configuration parameters to the
 * registered storage.
 *
 * @return 0 on success, non-zero on failure.
 */
int registry_save(void);

int registry_save_namespace(const registry_namespace_t *namespace);

int registry_save_schema(const registry_schema_t *schema);

int registry_save_instance(const registry_instance_t *instance);

int registry_save_group(const registry_instance_t *instance, const registry_group_t *group);

int registry_save_parameter(const registry_instance_t *instance,
                            const registry_parameter_t *parameter);

/**
 * @brief Registers a new storage as a source of configurations. Multiple
 * storages can be configured as sources at the same time. Configurations
 * will be loaded from all of them. If more than one storage contain values
 * for the same key, then only the value of the storage is used, that was
 * registered last.
 *
 * @param[in] src Pointer to the storage to register as source.
 */
#define REGISTRY_ADD_STORAGE_SOURCE(_storage_instance) \
        XFA_USE_CONST(registry_storage_instance_t *, _registry_storage_instances_src_xfa); \
        XFA_ADD_PTR(_registry_storage_instances_src_xfa, _storage_instance, _storage_instance, \
                    &_storage_instance)

extern const registry_storage_instance_t *_registry_storage_instance_dst;

/**
 * @brief Registers a new storage as a destination for saving configurations
 * Only one storage can be registered as destination at a time. If a
 * previous storage had been registered before it will be replaced by the
 * new one.
 *
 * @param[in] dst Pointer to the storage to register.
 */
#define REGISTRY_SET_STORAGE_DESTINATION(_storage_instance) \
        const registry_storage_instance_t *_registry_storage_instance_dst = &_storage_instance \

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_STORAGE_H */
/** @} */
