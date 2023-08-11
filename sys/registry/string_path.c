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
#include <string.h>

#define ENABLE_DEBUG (0)
#include "debug.h"
#include "kernel_defines.h"
#include "clist.h"
#include "registry.h"
#include "registry/util.h"
#include "registry/error.h"

#include "registry/string_path.h"

XFA_USE_CONST(registry_namespace_t *, _registry_namespaces_xfa);

static int _namespace_lookup(const char *path, registry_namespace_t **namespace)
{
    assert(path != NULL);
    assert(namespace != NULL);

    char *ptr = (char *)path;

    /* remove '/' character */
    ptr++;

    for (size_t i = 0; i < XFA_LEN(registry_namespace_t *, _registry_namespaces_xfa); i++) {
        const size_t name_length = strlen(_registry_namespaces_xfa[i]->name);

        /* check if length of path and name match */
        if (*(ptr + name_length) == '\0' ||
            *(ptr + name_length) == '/') {
            /* check if strings are equal */
            if (strncmp(ptr, _registry_namespaces_xfa[i]->name, name_length) == 0) {
                *namespace = _registry_namespaces_xfa[i];
                return name_length + 1; /* name_length + `/` character */
            }
        }
    }

    return -EINVAL;
}

static int _schema_lookup(const char *path, const registry_namespace_t *namespace,
                          registry_schema_t **schema)
{
    assert(path != NULL);
    assert(namespace != NULL);
    assert(schema != NULL);

    char *ptr = (char *)path;

    /* remove '/' character */
    ptr++;

    for (size_t i = 0; i < namespace->schemas_len; i++) {
        const size_t name_length = strlen(namespace->schemas[i]->name);

        /* check if length of path and name match */
        if (*(ptr + name_length) == '\0' ||
            *(ptr + name_length) == '/') {
            /* check if strings are equal */
            if (strncmp(ptr, namespace->schemas[i]->name, name_length) == 0) {
                *schema = (registry_schema_t *)namespace->schemas[i];
                return name_length + 1; /* name_length + `/` character */
            }
        }
    }

    return -EINVAL;
}

static int _instance_lookup(const char *path, registry_schema_t *schema,
                            registry_instance_t **instance)
{
    assert(path != NULL);
    assert(schema != NULL);
    assert(instance != NULL);

    char *ptr = (char *)path;

    /* remove '/' character */
    ptr++;

    clist_node_t *node = schema->instances.next;

    if (!node) {
        return -REGISTRY_ERROR_INSTANCE_NOT_FOUND;
    }

    do {
        node = node->next;

        const size_t name_length = strlen(container_of(node, registry_instance_t, node)->name);

        /* check if length of path and name match */
        if (*(ptr + name_length) == '\0' ||
            *(ptr + name_length) == '/') {
            /* check if strings are equal */
            if (strncmp(ptr, container_of(node, registry_instance_t, node)->name,
                        name_length) == 0) {
                *instance = container_of(node, registry_instance_t, node);
                return name_length + 1; /* name_length + `/` character */
            }
        }
    } while (node != schema->instances.next);

    return -EINVAL;
}

/* to string_path */
int registry_to_namespace_string_path(const registry_namespace_t *namespace, char *path)
{
    int size = snprintf(NULL, 0, "/%s", namespace->name);

    if (path != NULL) {
        return snprintf(path, size + 1, "/%s", namespace->name);
    }

    return size;
}

int registry_to_schema_string_path(const registry_schema_t *schema, char *path)
{
    int size = snprintf(NULL, 0, "/%s/%s", schema->namespace->name, schema->name);

    if (path != NULL) {
        return snprintf(path, size + 1, "/%s/%s", schema->namespace->name, schema->name);
    }

    return size;
}

int registry_to_instance_string_path(const registry_instance_t *instance, char *path)
{
    int size = snprintf(NULL, 0, "/%s/%s/%s", instance->schema->namespace->name,
                        instance->schema->name, instance->name);

    if (path != NULL) {
        return snprintf(path, size + 1, "/%s/%s/%s", instance->schema->namespace->name,
                        instance->schema->name, instance->name);
    }

    return size;
}

static int _internal_registry_to_group_string_path(const registry_group_t *current_group,
                                                   const registry_group_t *group, char *path)
{
    int size = snprintf(NULL, 0, "/%s", current_group->name);

    if (path != NULL) {
        snprintf(path, size + 1, "/%s", current_group->name);
    }

    if (group == current_group) {
        return size;
    }

    for (size_t i = 0; i < current_group->groups_len; i++) {
        int res = _internal_registry_to_group_string_path(current_group->groups[i], group,
                                                          path != NULL ? path + size : NULL);
        if (res >= 0) {
            return size + res;
        }
    }

    return -EINVAL;
}

int registry_to_group_string_path(const registry_instance_t *instance,
                                  const registry_group_t *group, char *path)
{
    int size = snprintf(NULL, 0, "/%s/%s/%s", instance->schema->namespace->name,
                        instance->schema->name, instance->name);

    if (path != NULL) {
        snprintf(path, size + 1, "/%s/%s/%s", instance->schema->namespace->name,
                 instance->schema->name, instance->name);
    }

    for (size_t i = 0; i < instance->schema->groups_len; i++) {
        int res = _internal_registry_to_group_string_path(instance->schema->groups[i], group,
                                                          path != NULL ? path + size : NULL);
        if (res >= 0) {
            return size += res;
        }
    }

    return -EINVAL;
}

static int _internal_registry_to_parameter_string_path(const registry_group_t *current_group,
                                                       const registry_parameter_t *parameter,
                                                       char *path)
{
    int size = snprintf(NULL, 0, "/%s", current_group->name);

    if (path != NULL) {
        snprintf(path, size + 1, "/%s", current_group->name);
    }

    /* check if the parameter is a child of this group */
    for (size_t i = 0; i < current_group->parameters_len; i++) {
        if (current_group->parameters[i] == parameter) {
            int sub_size = snprintf(NULL, 0, "/%s", current_group->parameters[i]->name);

            if (path != NULL) {
                snprintf(path + size, sub_size + 1, "/%s", current_group->parameters[i]->name);
            }

            return size + sub_size;
        }
    }

    /* check if the parameter is the child of a subgroup */
    for (size_t i = 0; i < current_group->groups_len; i++) {
        int res = _internal_registry_to_parameter_string_path(current_group->groups[i], parameter,
                                                              path != NULL ? path + size : NULL);
        if (res >= 0) {
            return size + res;
        }
    }

    return -EINVAL;
}

int registry_to_parameter_string_path(const registry_instance_t *instance,
                                      const registry_parameter_t *parameter, char *path)
{
    int size = snprintf(NULL, 0, "/%s/%s/%s", instance->schema->namespace->name,
                        instance->schema->name, instance->name);

    if (path != NULL) {
        snprintf(path, size + 1, "/%s/%s/%s", instance->schema->namespace->name,
                 instance->schema->name, instance->name);
    }

    /* check if the parameter is a child of this schema */
    for (size_t i = 0; i < instance->schema->parameters_len; i++) {
        if (instance->schema->parameters[i] == parameter) {
            int sub_size = snprintf(NULL, 0, "/%s", instance->schema->parameters[i]->name);

            if (path != NULL) {
                snprintf(path + size, sub_size + 1, "/%s", instance->schema->parameters[i]->name);
            }

            return size + sub_size;
        }
    }

    /* check if the parameter is the child of a group */
    for (size_t i = 0; i < instance->schema->groups_len; i++) {
        int res = _internal_registry_to_parameter_string_path(instance->schema->groups[i],
                                                              parameter,
                                                              path != NULL ? path + size : NULL);
        if (res >= 0) {
            return size += res;
        }
    }

    return -EINVAL;
}

/* from string_path */
int registry_from_namespace_string_path(const char *path, registry_namespace_t **namespace)
{
    int res = _namespace_lookup(path, namespace);

    if (res < 0) {
        return res;
    }

    return 0;
}

int registry_from_schema_string_path(const char *path, registry_namespace_t **namespace,
                                     registry_schema_t **schema)
{
    size_t processed_path_len = 0;

    registry_namespace_t *found_namespace;
    int res = _namespace_lookup(path, &found_namespace);

    if (res >= 0) {
        processed_path_len += res;

        if (namespace != NULL) {
            *namespace = found_namespace;
        }

        res = _schema_lookup(path + processed_path_len, found_namespace, schema);
    }

    if (res < 0) {
        return res;
    }

    return 0;
}

int registry_from_instance_string_path(const char *path, registry_namespace_t **namespace,
                                       registry_schema_t **schema, registry_instance_t **instance)
{
    size_t processed_path_len = 0;

    registry_namespace_t *found_namespace;
    int res = _namespace_lookup(path, &found_namespace);

    if (res >= 0) {
        processed_path_len += res;

        if (namespace != NULL) {
            *namespace = found_namespace;
        }

        registry_schema_t *found_schema;
        res = _schema_lookup(path + processed_path_len, found_namespace, &found_schema);

        if (res >= 0) {
            processed_path_len += res;

            if (schema != NULL) {
                *schema = found_schema;
            }

            res = _instance_lookup(path + processed_path_len, found_schema, instance);
        }
    }

    if (res < 0) {
        return res;
    }

    return 0;
}

int registry_from_group_string_path(const char *path, registry_namespace_t **namespace,
                                    registry_schema_t **schema, registry_instance_t **instance,
                                    registry_group_t **group)
{
    (void)path;
    (void)namespace;
    (void)schema;
    (void)instance;
    (void)group;
    // TODO
    return 0;
}

int registry_from_parameter_string_path(const char *path, registry_namespace_t **namespace,
                                        registry_schema_t **schema, registry_instance_t **instance,
                                        registry_parameter_t **parameter)
{
    (void)path;
    (void)namespace;
    (void)schema;
    (void)instance;
    (void)parameter;
    // TODO
    return 0;
}

int registry_from_group_or_parameter_string_path(const char *path,
                                                 registry_string_path_type_t *path_type,
                                                 registry_namespace_t **namespace,
                                                 registry_schema_t **schema,
                                                 registry_instance_t **instance,
                                                 registry_group_t **group,
                                                 registry_parameter_t **parameter)
{
    (void)path;
    (void)path_type;
    (void)namespace;
    (void)schema;
    (void)instance;
    (void)group;
    (void)parameter;
    // TODO
    return 0;
}
