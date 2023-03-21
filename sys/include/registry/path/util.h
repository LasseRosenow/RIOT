/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_path_util RIOT Registry Path utilities
 * @ingroup     sys
 * @brief       RIOT Registry Path Util module providing utility functions
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#ifndef REGISTRY_PATH_UTIL_H
#define REGISTRY_PATH_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"
#include "registry/path.h"

int registry_path_util_parse_string_path(const char *string_path,
                                         registry_path_t *registry_path,
                                         registry_id_t *path_items_buf);

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* REGISTRY_PATH_UTIL_H */
