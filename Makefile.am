AM_CPPFLAGS = -I$(srcdir)/src \
	      -DENVD='"$(sysconfdir)/env.d"' \
	      -DLIB='"$(libdir)"'
AM_CFLAGS = -Wall -Wextra

bin_PROGRAMS = go
go_SOURCES = src/wrapper.c src/common.c src/common.h

TESTS = go-wrapper-tests
check_PROGRAMS = go-wrapper-tests
go_wrapper_tests_SOURCES = src/tests.c src/common.c src/common.h
go_wrapper_tests_CFLAGS = $(AM_CFLAGS) $(GLIB_CFLAGS) -Werror
go_wrapper_tests_LDADD = $(GLIB_LIBS)

eselectdir = $(datadir)/eselect/modules
nodist_eselect_DATA = go.eselect
