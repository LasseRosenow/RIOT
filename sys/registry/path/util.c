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

#include <stdlib.h>

#define ENABLE_DEBUG (0)
#include "debug.h"

#include "registry/path/util.h"

int registry_path_util_parse_string_path(const char *string_path,
                                         registry_path_t *registry_path)
{
    char *ptr = (char *)string_path;

    int i = 0;

    while (*ptr != '\0') {
        registry_id_t id = strtol(ptr, &ptr, 10);

        switch (i) {
        case 0: *(registry_id_t *)registry_path->namespace_id = id; break;
        case 1: *(registry_id_t *)registry_path->schema_id = id; break;
        case 2: *(registry_id_t *)registry_path->instance_id = id; break;
        case 3: *(registry_id_t *)registry_path->resource_id = id; break;
        }

        if (*ptr != '\0') {
            ptr++;
        }

        i++;
    }

    return 0;
}
