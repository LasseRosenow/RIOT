/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_error RIOT Registry Error
 * @ingroup     sys
 * @brief       RIOT Registry module providing all possible error codes
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_ERROR_H
#define REGISTRY_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef const enum {
    REGISTRY_ERROR_NONE = 1000,
    REGISTRY_ERROR_NO_DST_STORAGE,
} registry_error_t;

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_ERROR_H */
/** @} */
