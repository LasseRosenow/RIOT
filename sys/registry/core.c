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

clist_node_t _registry_namespaces;

int registry_register_namespace(const registry_namespace_t *namespace)
{
    assert(namespace != NULL);

    /* add namespace to list */
    clist_rpush((clist_node_t *)&_registry_namespaces, (clist_node_t *)&namespace->node);

    return 0;
}

int registry_register_schema_instance(const registry_schema_t *schema,
                                      const registry_instance_t *instance)
{
    assert(schema != NULL);
    assert(instance != NULL);

    /* add instance to schema */
    clist_rpush((clist_node_t *)&schema->instances, (clist_node_t *)&instance->node);

    return 0;
}

int registry_get(const registry_schema_t *schema, const registry_instance_t *instance,
                 const registry_schema_item_t *parameter, registry_value_t *value)
{
    assert(schema != NULL);
    assert(instance != NULL);
    assert(value != NULL);

    /* call handler to get pointer to registry internal value buffer and length */
    void *intern_val = NULL;
    size_t intern_val_len;

    schema->mapping(parameter->id, instance, &intern_val, &intern_val_len);

    /* update buf pointer in registry_value_t to point to the value inside the registry and set buf_len */
    value->type = parameter->type;
    value->buf = intern_val;
    value->buf_len = intern_val_len;

    return 0;
}

int registry_set(const registry_schema_t *schema, const registry_instance_t *instance,
                 const registry_schema_item_t *parameter, const registry_value_t *value)
{
    /* get pointer to registry internal value buffer and length */
    void *intern_val = NULL;
    size_t intern_val_len;

    schema->mapping(parameter->id, instance, &intern_val, &intern_val_len);

    /* check if val_type is compatible with param_meta->value.parameter.type */
    if (value->type != parameter->type) {
        return -EINVAL;
    }
    else {
        /* call handler to apply the new value to the correct parameter in the instance of the schema */
        memcpy(intern_val, value->buf, intern_val_len);
    }

    return 0;
}

int registry_commit(const registry_commit_data_t data, const registry_commit_data_type_t data_type)
{

}

int registry_export(const registry_export_cb_t *export_cb, const registry_export_data_t data,
                    const registry_export_data_type_t data_type, const int recursion_depth,
                    const void *context)
{

}
