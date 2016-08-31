AC_PREREQ([2.69])
AC_INIT([eselect-go],[0.0.1],[https://github.com/coreos/bugs/issues])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign 1.13 -Wall -Wno-portability subdir-objects])

AC_PROG_CC

AC_USE_SYSTEM_EXTENSIONS

# Only used for unit tests.
AM_PATH_GLIB_2_0([2.20.0])

# Create output files.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile go.eselect])

AC_OUTPUT
