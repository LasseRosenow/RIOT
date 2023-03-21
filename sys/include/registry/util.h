/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_util RIOT Registry utilities
 * @ingroup     sys
 * @brief       RIOT Registry Util module providing utility functions
 * @{
 *
 * @file
 *
 * @author      Leandro Lanzieri <leandro.lanzieri@haw-hamburg.de>
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#ifndef REGISTRY_UTIL_H
#define REGISTRY_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"

void _debug_print_value(const registry_value_t *value);

const registry_namespace_t *registry_util_namespace_lookup(const registry_id_t namespace_id);

const registry_schema_t *registry_util_schema_lookup(const registry_namespace_t *namespace,
                                                     const registry_id_t schema_id);

const registry_instance_t *registry_util_instance_lookup(const registry_schema_t *schema,
                                                         const registry_id_t instance_id);

/**
 * @brief Convenience function to parse a configuration parameter value from
 * a string. The type of the parameter must be known and must not be `bytes`.
 * To parse the string to bytes @ref registry_bytes_from_str() function must be
 * used.
 *
 * @param[in] src Pointer of the input value
 * @param[out] dest Pointer to the output buffer
 * @param[in] dest_len Length of @p dest
 * @param[in] dest_type Type of the output value
 * @return 0 on success, non-zero on failure
 */
int registry_util_convert_str_to_value(const char *src, void *dest, const size_t dest_len,
                                       const registry_type_t dest_type);

/**
 * @brief Convenience function to transform a configuration parameter value into
 * a string, when the parameter is not of `bytes` type, in this case
 * @ref registry_str_from_bytes() should be used. This is used for example to
 * implement the `get` or `export` functions.
 *
 * @param[in] src Pointer to the value to be converted
 * @param[out] dest Buffer to store the output string
 * @param[in] dest_len Length of @p buf
 * @return Length of string value on success, non-zero on failure
 */
int registry_util_convert_value_to_str(const registry_value_t *src, char *dest,
                                       const size_t dest_len);

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* REGISTRY_UTIL_H */
