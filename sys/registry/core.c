/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_registry RIOT Registry
 * @ingroup     module_registry
 * @brief       RIOT Registry for runtime configuration of modules
 * @{
 *
 * @file
 *
 * @author      Leandro Lanzieri <leandro.lanzieri@haw-hamburg.de>
 * @author      Lasse Rosenow <lasse.rosenow@haw-hamburg.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "clist.h"

#include "registry.h"
#include "registry/util.h"

/* Implementation of the module */

registry_namespace_t registry_namespace_sys = {
    .id = REGISTRY_NAMESPACE_SYS,
    .name = "sys",
    .description = "List of RIOT sys schemas.",
    .schemas = { .next = NULL },
};

registry_namespace_t registry_namespace_app = {
    .id = REGISTRY_NAMESPACE_APP,
    .name = "app",
    .description = "List of custom app schemas.",
    .schemas = { .next = NULL },
};

int registry_register_schema(const registry_namespace_id_t namespace_id,
                             const registry_schema_t *schema)
{
    assert(schema != NULL);

    /* find namespace with correct namespace id */
    registry_namespace_t *namespace = _namespace_lookup(namespace_id);

    if (!namespace) {
        return -EINVAL;
    }

    clist_rpush((clist_node_t *)&namespace->schemas, (clist_node_t *)&(schema->node));

    return 0;
}

int registry_register_schema_instance(const registry_namespace_id_t namespace_id,
                                      const registry_id_t schema_id,
                                      const registry_instance_t *instance)
{
    assert(instance != NULL);

    /* find namespace with correct namespace id */
    registry_namespace_t *namespace = _namespace_lookup(namespace_id);

    if (!namespace) {
        return -EINVAL;
    }

    /* find schema with correct schema_id */
    clist_node_t *node = namespace->schemas.next;

    do {
        node = node->next;
        registry_schema_t *schema = container_of(node, registry_schema_t, node);

        /* check if schema has correct schema_id */
        if (schema->id == schema_id) {
            /* add instance to schema */
            clist_rpush((clist_node_t *)&(schema->instances), (clist_node_t *)&instance->node);

            /* count instance index */
            return clist_count(&schema->instances) - 1;
        }
    } while (node != namespace->schemas.next);

    return -EINVAL;
}
