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
