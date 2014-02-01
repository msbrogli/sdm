AUTOMAKE_OPTIONS = foreign

CFLAGS=-Wall -fPIC -DVERSION=\"$(VERSION)\" -DPROG="\"$(PACKAGE)\""
LDFLAGS=-lpthread

bin_PROGRAMS = main
main_SOURCES = src/main.c src/core/memory.c src/core/bitstring.c
