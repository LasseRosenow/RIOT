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
 *
 * @}
 */

#include <errno.h>
#include <assert.h>
#include <stdlib.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "clist.h"
#include "registry.h"
#include "registry/util.h"
#include "registry/error.h"

#include "registry/string_path.h"

// TODO implement this file

/* to string_path */
int registry_to_namespace_string_path(const registry_namespace_t *namespace, char *path)
{
    (void)namespace;
    (void)path;
    return 0;
}

int registry_to_schema_string_path(const registry_schema_t *schema, char *path)
{
    (void)schema;
    (void)path;
    return 0;
}

int registry_to_instance_string_path(const registry_instance_t *instance, char *path)
{
    (void)instance;
    (void)path;
    return 0;
}

int registry_to_resource_string_path(const registry_instance_t *instance,
                                     const registry_resource_t *resource, char *path)
{
    (void)instance;
    (void)resource;
    (void)path;
    return 0;
}

/* from string_path */
int registry_from_namespace_string_path(const char *path,
                                        registry_namespace_t **namespace)
{
    (void)path;
    (void)namespace;
    return 0;
}

int registry_from_schema_string_path(const char *path,
                                     registry_namespace_t **namespace, registry_schema_t **schema)
{
    (void)path;
    (void)namespace;
    (void)schema;
    return 0;
}

int registry_from_instance_string_path(const char *path,
                                       registry_namespace_t **namespace, registry_schema_t **schema,
                                       registry_instance_t **instance)
{
    (void)path;
    (void)namespace;
    (void)schema;
    (void)instance;
    return 0;
}

int registry_from_resource_string_path(const char *path,
                                       registry_namespace_t **namespace, registry_schema_t **schema,
                                       registry_instance_t **instance,
                                       registry_resource_t **resource)
{
    (void)path;
    (void)namespace;
    (void)schema;
    (void)instance;
    (void)resource;
    return 0;
}
