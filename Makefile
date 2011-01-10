CINCLUDE=.
CFLAGS=-Wall -O3
FLAGS=
CC=gcc

OBJS=sdm/bitstring.o sdm/hardlocation.o sdm/memory.o

.c.o:
	$(CC) $(CFLAGS) -I$(CINCLUDE) -c -o $*.o $*.c

all: $(OBJS)
	echo "All done."

tests: $(OBJS) test_memory

#test_bitstring: $(OBJS)
#	$(CC) $(CFLAGS) -I$(CINCLUDE) $(TESTLIB) -o test_bistring $(OBJS) test_bitstring.cc

test_memory: $(OBJS)
	$(CC) $(CFLAGS) -I$(CINCLUDE) $(TESTLIB) -o test_memory $(OBJS) test_memory.c

clean:
	-rm -f sdm/*.o

