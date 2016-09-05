/* Copyright (C) 2016  CoreOS,Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef ESELECT_GO_COMMON_H__
#define ESELECT_GO_COMMON_H__

#include "config.h"
#include <sys/types.h>

void set_program_name(const char *path);
void set_library_dir(const char *path);
void set_config_file(const char *path);
size_t program_path(char *path, const char *dir, size_t path_len);
int try_latest(char *dir, size_t dir_len);
int try_config(char *dir, size_t dir_len);
int try_environ(char *dir, size_t dir_len);

#endif
