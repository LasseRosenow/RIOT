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
 * @brief       RIOT Registry String Path module providing a API to access configuration parameter via a string path
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

#include "registry.h"

/* to string_path */
int registry_to_namespace_string_path(const registry_namespace_t *namespace, char *path);

int registry_to_schema_string_path(const registry_schema_t *schema, char *path);

int registry_to_instance_string_path(const registry_instance_t *instance, char *path);

int registry_to_resource_string_path(const registry_instance_t *instance,
                                     const registry_resource_t *resource, char *path);

/* from string_path */
int registry_from_namespace_string_path(const char *path,
                                        registry_namespace_t **namespace);

int registry_from_schema_string_path(const char *path,
                                     registry_namespace_t **namespace, registry_schema_t **schema);

int registry_from_instance_string_path(const char *path,
                                       registry_namespace_t **namespace, registry_schema_t **schema,
                                       registry_instance_t **instance);

int registry_from_resource_string_path(const char *path,
                                       registry_namespace_t **namespace, registry_schema_t **schema,
                                       registry_instance_t **instance,
                                       registry_resource_t **resource);

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_STRING_PATH_H */
/** @} */
