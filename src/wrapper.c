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

#include "common.h"

#include <limits.h>
#include <unistd.h>

/* 127 is the standard return code for "command not found" */
#define EXIT_ERROR 127

int main(int argc __attribute__ ((unused)), char **argv)
{
	char target_dir[NAME_MAX + 1], target_path[PATH_MAX];

	set_program_name(argv[0]);

	if (try_environ(target_dir, sizeof(target_dir)) ||
	    try_config(target_dir, sizeof(target_dir)) ||
	    try_latest(target_dir, sizeof(target_dir))) {
		program_path(target_path, target_dir, sizeof(target_path));
		argv[0] = target_path;
		execv(argv[0], argv);
	}

	return EXIT_ERROR;
}
