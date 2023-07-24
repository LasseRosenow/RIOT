/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry_namespace_sys RIOT Registry Sys Namespace
 * @ingroup     sys
 * @brief       RIOT Registry Namespace Sys module providing common sys configuration schemas for the RIOT Registry sys module
 * @{
 *
 * @file
 *
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 */

#ifndef REGISTRY_NAMESPACE_SYS_H
#define REGISTRY_NAMESPACE_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "registry.h"
#include "registry/namespace_sys/rgb_led.h"

/* Sys namespace */

extern const registry_namespace_t registry_sys;

#ifdef __cplusplus
}
#endif

#endif /* REGISTRY_NAMESPACE_SYS_H */
/** @} */
