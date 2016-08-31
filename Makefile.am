AM_CPPFLAGS = -I$(srcdir)/src \
	      -DENVD='"$(sysconfdir)/env.d"' \
	      -DLIB='"$(libdir)"'
AM_CFLAGS = -Wall -Wextra

bin_PROGRAMS = go
go_SOURCES = src/wrapper.c src/common.c src/common.h

eselectdir = $(datadir)/eselect/modules
nodist_eselect_DATA = go.eselect
