CINCLUDE=.
CFLAGS=-Wall -O3
CC=gcc

OBJS=src/bitstring.o src/hardlocation.o src/memory.o src/memory_thread.o #src/memory_opencl.o
TESTS=test_memory test_write test_memory_thread

LIB=python/libsdm.so

.PHONY: all lib tests clean rsync

all: lib

lib: $(LIB)

rsync:
	rsync --exclude-from '.rsyncignore' -av --delete . ~/Dropbox/Dr\ K/sdm/

%.o: %.c %.h src/common.h
	$(CC) $(CFLAGS) -fPIC -I$(CINCLUDE) -c -o $*.o $*.c

$(LIB): $(OBJS)
	gcc -shared -framework OpenCL -lpthread -o $@ $(OBJS)

test_%: test_%.c $(OBJS)
	$(CC) $(CFLAGS) -I$(CINCLUDE) $(TESTLIB) -o test_$* $(OBJS) test_$*.c

tests: $(TESTS)

clean:
	rm -f $(OBJS) $(LIB) $(TESTS)

