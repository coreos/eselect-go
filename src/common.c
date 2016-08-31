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

#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef LIB
#define LIB "/usr/lib"
#endif

#ifndef ENVD
#define ENVD "/etc/env.d"
#endif

/* Program name, set based on argv[0], e.g. "go" or "gofmt" */
static char program_name[NAME_MAX + 1];

/* Library path where Go can be found, e.g. "/usr/lib" */
static char library_dir[PATH_MAX] = LIB;

/* Config file written by go.eselect, e.g. "/etc/env.d/go/config" */
static char config_file[PATH_MAX] = ENVD "/go/config";

static void strxcpy(char *dest, const char *src, size_t dest_len)
{
	strncpy(dest, src, dest_len - 1);
	dest[dest_len - 1] = '\0';
}

void set_program_name(const char *path)
{
	const char *slash = strrchr(path, '/');
	if (slash)
		strxcpy(program_name, slash + 1, sizeof(program_name));
	else
		strxcpy(program_name, path, sizeof(program_name));
}

/* Just for unit tests.  */
void set_library_dir(const char *path)
{
	strxcpy(library_dir, path, sizeof(library_dir));
}

/* Just for unit tests.  */
void set_config_file(const char *path)
{
	strxcpy(config_file, path, sizeof(config_file));
}

size_t program_path(char *path, const char *dir, size_t path_len)
{
	return snprintf(path, path_len,
			"%s/%s/bin/%s", library_dir, dir, program_name);
}

static int filter_program(const struct dirent *dir)
{
	char path[PATH_MAX];
	struct stat st;

	if (dir->d_type != DT_DIR && dir->d_type != DT_UNKNOWN)
		return 0;

	program_path(path, dir->d_name, sizeof(path));
	if (stat(path, &st) != 0)
		return 0;
	return S_ISREG(st.st_mode);
}

int try_latest(char *dir, size_t dir_len)
{
	struct dirent **namelist = NULL;
	int n;

	n = scandir(library_dir, &namelist, filter_program, versionsort);
	if (n <= 0) {
		free(namelist);
		return 0;
	}

	strxcpy(dir, namelist[n - 1]->d_name, dir_len);
	while (n--)
		free(namelist[n]);
	free(namelist);
	return 1;
}

int try_config(char *dir, size_t dir_len)
{
	char d[NAME_MAX + 1];
	ssize_t n;
	int fd;

	if ((fd = open(config_file, O_RDONLY | O_CLOEXEC)) < 0)
		return 0;
	if ((n = read(fd, d, sizeof(d) - 1)) < 0)
		return 0;

	if (d[n - 1] == '\n')
		n--;
	if (n == 0)
		return 0;
	d[n] = '\0';

	strxcpy(dir, d, dir_len);
	return 1;
}

int try_environ(char *dir, size_t dir_len)
{
	char *d = getenv("EGO");
	if (!d || !strlen(d))
		return 0;
	strxcpy(dir, d, dir_len);
	return 1;
}
