CINCLUDE=.
CFLAGS=-Wall -O3
FLAGS=
CC=gcc

OBJS=sdm/bitstring.o sdm/hardlocation.o sdm/memory.o

%.o: %.c %.h sdm/common.h
	$(CC) $(CFLAGS) -I$(CINCLUDE) -c -o $*.o $*.c

all: $(OBJS)
	echo "All done."

tests: $(OBJS) test_memory

#test_bitstring: $(OBJS)
#	$(CC) $(CFLAGS) -I$(CINCLUDE) $(TESTLIB) -o test_bistring $(OBJS) test_bitstring.cc

test_%: test_%.c $(OBJS)
	$(CC) $(CFLAGS) -I$(CINCLUDE) $(TESTLIB) -o test_$* $(OBJS) test_$*.c

tests: test_*

clean:
	-rm -f sdm/*.o

