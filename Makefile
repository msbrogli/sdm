CINCLUDE=.
CFLAGS=-Wall -O3
CC=gcc

OBJS=sdm/bitstring.o sdm/memory.o sdm/memory_thread.o

LIB=python/libsdm.so

.PHONY: all lib tests clean rsync

all: lib

lib: $(LIB)

%.o: %.c %.h sdm/common.h
	$(CC) $(CFLAGS) -fPIC -I$(CINCLUDE) -c -o $*.o $*.c

$(LIB): $(OBJS)
	gcc -shared -framework OpenCL -lpthread -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIB) $(TESTS)

