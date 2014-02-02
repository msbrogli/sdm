#include <stdio.h>
#include <stdlib.h>
#include "src/core/memory.h"
#include "src/core/memory_thread.h"

extern unsigned int bs_dimension;
extern unsigned int sdm_radius;

int main(int argv, char **argc) {
	bitstring* address;
	bitstring* data;
	int i, lo, hi;

	struct sdm_memory mem;
	mem.sample = 1000000;

	if (argv == 4) {
		bs_dimension = atoi(argc[1]);
		mem.sample = atoi(argc[2]);
		sdm_radius = atoi(argc[3]);
	}

	sdm_initialize(&mem);

	//printf("All initialized.\n\n");

	printf("%d %d %d\n", bs_dimension, mem.sample, sdm_radius);

	address = bs_init_random(bs_alloc());
	data = bs_init_random(bs_alloc());

	lo = 0;
	hi = bs_dimension;
	for(i=lo; i<=hi; i++) {
		sdm_radius = i;
		sdm_thread_write(&mem, address, data);
		if (i%10 == 0) {
			printf("@@ %d\n", i);
			fflush(stdout);
		}
	}

	return 0;
}
