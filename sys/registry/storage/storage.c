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
 *
 * @}
 */

#include <assert.h>

#define ENABLE_DEBUG (0)
#include "debug.h"

#include "registry/storage.h"

const registry_storage_instance_t *_storage_dst;
clist_node_t _storage_srcs;

void registry_register_storage_src(const registry_storage_instance_t *src)
{
    assert(src != NULL);
    clist_rpush((clist_node_t *)&_storage_srcs, (clist_node_t *)&(src->node));
}

void registry_register_storage_dst(const registry_storage_instance_t *dst)
{
    assert(dst != NULL);
    _storage_dst = dst;
}
