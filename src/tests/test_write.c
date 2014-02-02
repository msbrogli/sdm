#include <stdio.h>
#include <stdlib.h>
#include "src/core/memory.h"
#include "src/core/memory_thread.h"

extern unsigned int bs_dimension;
extern unsigned int sdm_sample;
extern unsigned int sdm_radius;

int main(int argv, char **argc) {
	bitstring* address;
	bitstring* data;
	bitstring* read;

	struct sdm_memory mem;
	mem.sample = 1000000;

	if (argv == 4) {
		bs_dimension = atoi(argc[1]);
		mem.sample = atoi(argc[2]);
		sdm_radius = atoi(argc[3]);
	}

	sdm_initialize(&mem);

	printf("All initialized.\n\n");

	printf("%d %d %d\n", bs_dimension, mem.sample, sdm_radius);

	address = bs_init_random(bs_alloc());
	data = bs_init_random(bs_alloc());

	sdm_thread_write(&mem, address, data);
	read = sdm_thread_read(&mem, address);

	if (read != NULL) {
		bs_print(data);
		bs_print(read);
		printf("Distance = %d\n", bs_distance(data, read));
	}

	return 0;
}
