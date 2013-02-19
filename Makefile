CINCLUDE=.
CFLAGS=-Wall -O3 -mtune=native -fforce-addr -funroll-loops -frerun-cse-after-loop -frerun-loop-opt -falign-functions=4
CC=gcc

OBJS=sdm/bitstring.o sdm/hardlocation.o sdm/memory.o sdm/memory_thread.o sdm/memory_opencl.o
TESTS=test_memory test_write

LIB=python/libsdm.so

.PHONY: all lib tests clean rsync

all: lib

lib: $(LIB)

rsync:
	rsync --exclude-from '.rsyncignore' -av --delete . ~/Dropbox/Dr\ K/sdm/

%.o: %.c %.h sdm/common.h
	$(CC) $(CFLAGS) -fPIC -I$(CINCLUDE) -c -o $*.o $*.c

$(LIB): $(OBJS)
	gcc -shared -framework OpenCL -lpthread -o $@ $(OBJS)

test_%: test_%.c $(OBJS)
	$(CC) $(CFLAGS) -I$(CINCLUDE) $(TESTLIB) -o test_$* $(OBJS) test_$*.c

tests: $(TESTS)

clean:
	rm -f $(OBJS) $(LIB) $(TESTS)

