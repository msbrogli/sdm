AUTOMAKE_OPTIONS = foreign

CFLAGS=-Wall -DVERSION=\"$(VERSION)\" -DPROG="\"$(PACKAGE)\""
LDFLAGS=-lpthread
ACLOCAL_AMFLAGS=-I m4

bin_PROGRAMS = main
main_SOURCES = src/main.c
main_LDADD = python/libsdm.la
#main_SOURCES = src/main.c \
#               src/core/memory.c src/core/memory.h \
#               src/core/bitstring.c src/core/bitstring.h

lib_LTLIBRARIES = python/libsdm.la
python_libsdm_la_SOURCES = src/core/memory.c src/core/memory.h \
						   src/core/bitstring.c src/core/bitstring.h \
						   src/core/memory_thread.c src/core/memory_thread.h
python_libsdm_la_LDFLAGS = -fPIC

maintainer-clean-local:
	make clean
	rm -rf .deps/ autom4te.cache/ m4/
	rm -f Makefile Makefile.in
	rm -f config.h.in config.h
	rm -f aclocal.m4
	rm -f config.guess config.log config.status config.sub
	rm -f configure
	rm -f depcomp
	rm -f install-sh
	rm -f libtool
	rm -f ltmain.sh
	rm -f missing
	rm -f python/.dirstamp
	rm -f stamp-h1
