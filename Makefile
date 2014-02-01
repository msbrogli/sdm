CINCLUDE=.
CFLAGS=-Wall -O3
CC=gcc

OBJS=src/core/bitstring.o src/core/memory.o src/core/memory_thread.o

LIB=python/libsdm.so

.PHONY: all lib tests clean rsync

all: lib

lib: $(LIB)

%.o: %.c %.h src/core/common.h
	$(CC) $(CFLAGS) -fPIC -I$(CINCLUDE) -c -o $*.o $*.c

$(LIB): $(OBJS)
	gcc -shared -lpthread -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIB) $(TESTS)

