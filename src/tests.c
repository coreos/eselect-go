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

#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>

#include <glib.h>

static void test_program_path()
{
	char path[PATH_MAX];

	set_library_dir("/fake/lib");
	set_program_name("app1");
	program_path(path, "dir1", sizeof(path));
	g_assert_cmpstr(path, ==, "/fake/lib/dir1/bin/app1");

	program_path(path, "dir2", sizeof(path));
	g_assert_cmpstr(path, ==, "/fake/lib/dir2/bin/app1");

	set_program_name("app2");
	program_path(path, "dir2", sizeof(path));
	g_assert_cmpstr(path, ==, "/fake/lib/dir2/bin/app2");
}

static void test_try_environ()
{
	char dir[NAME_MAX + 1];

	g_assert(unsetenv("EGO") == 0);
	g_assert(try_environ(dir, sizeof(dir)) == 0);
	g_assert(setenv("EGO", "", 1) == 0);
	g_assert(try_environ(dir, sizeof(dir)) == 0);
	g_assert(setenv("EGO", "dir1", 1) == 0);
	g_assert(try_environ(dir, sizeof(dir)) == 1);
	g_assert_cmpstr(dir, ==, "dir1");
	g_assert(unsetenv("EGO") == 0);
}

static void newtemp(char *tmpl)
{
	int fd = mkstemp(tmpl);
	g_assert(fd >= 0);
	g_assert(close(fd) == 0);
}

static void test_try_config()
{
	GError *err = NULL;
	char tmp[] = "/tmp/wrapper-config-XXXXXX";
	char dir[NAME_MAX + 1];

	set_config_file("/this/path/does/not/exist");
	g_assert(try_config(dir, sizeof(dir)) == 0);

	newtemp(tmp);
	set_config_file(tmp);
	g_assert(try_config(dir, sizeof(dir)) == 0);

	g_file_set_contents(tmp, "dir1\n", 5, &err);
	g_assert_no_error(err);
	g_assert(try_config(dir, sizeof(dir)) == 1);
	g_assert_cmpstr(dir, ==, "dir1");

	g_file_set_contents(tmp, "dir2", 4, &err);
	g_assert_no_error(err);
	g_assert(try_config(dir, sizeof(dir)) == 1);
	g_assert_cmpstr(dir, ==, "dir2");

	g_assert(unlink(tmp) == 0);
}

static void fakeapp(const char *lib, const char *dir, const char *app)
{
	char *bin = g_strconcat(lib, "/", dir, "/bin", NULL);
	char *path = g_strconcat(bin, "/", app, NULL);
	int fd;

	g_assert(g_mkdir_with_parents(bin, 0700) == 0);
	fd = open(path, O_WRONLY | O_CREAT | O_CLOEXEC, 0600);
	g_assert(fd >= 0);
	g_assert(close(fd) == 0);

	g_free(bin);
	g_free(path);
}

/* Does glib really not provide this? :-( */
static void rmdirs(char *path)
{
	GError *err = NULL;
	char *argv[] = { "rm", "-rf", path, NULL };
	g_spawn_sync(NULL, argv, NULL, G_SPAWN_SEARCH_PATH |
		     G_SPAWN_STDOUT_TO_DEV_NULL |
		     G_SPAWN_STDERR_TO_DEV_NULL,
		     NULL, NULL, NULL, NULL, NULL, &err);
	g_assert_no_error(err);
}

static void test_try_latest()
{
	char tmp[] = "/tmp/wrapper-latest-XXXXXX";
	char dir[NAME_MAX + 1];

	g_assert(mkdtemp(tmp) != NULL);
	set_library_dir(tmp);
	set_program_name("app1");
	g_assert(try_latest(dir, sizeof(dir)) == 0);

	fakeapp(tmp, "dir1", "app1");
	g_assert(try_latest(dir, sizeof(dir)) == 1);
	g_assert_cmpstr(dir, ==, "dir1");

	fakeapp(tmp, "dir2", "app1");
	g_assert(try_latest(dir, sizeof(dir)) == 1);
	g_assert_cmpstr(dir, ==, "dir2");

	fakeapp(tmp, "dir3", "app2");
	g_assert(try_latest(dir, sizeof(dir)) == 1);
	g_assert_cmpstr(dir, ==, "dir2");

	fakeapp(tmp, "dir10", "app1");
	g_assert(try_latest(dir, sizeof(dir)) == 1);
	g_assert_cmpstr(dir, ==, "dir10");

	set_program_name("app2");
	g_assert(try_latest(dir, sizeof(dir)) == 1);
	g_assert_cmpstr(dir, ==, "dir3");

	set_program_name("app3");
	g_assert(try_latest(dir, sizeof(dir)) == 0);

	rmdirs(tmp);
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");

	g_test_init(&argc, &argv, NULL);
	g_test_add_func("/wrapper/program_path", test_program_path);
	g_test_add_func("/wrapper/try_environ", test_try_environ);
	g_test_add_func("/wrapper/try_config", test_try_config);
	g_test_add_func("/wrapper/try_latest", test_try_latest);

	return g_test_run();
}
