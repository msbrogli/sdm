#include <stdio.h>
#include <stdlib.h>
#include "src/memory.h"

extern unsigned int bs_dimension;
extern unsigned int sdm_sample;
extern unsigned int sdm_radius;

int main(int argv, char **argc) {
	bitstring* address;
	bitstring* data;
	bitstring* read;

	if (argv == 4) {
		bs_dimension = atoi(argc[1]);
		sdm_sample = atoi(argc[2]);
		sdm_radius = atoi(argc[3]);
	}

	bs_initialize();
	hl_initialize();
	sdm_initialize();

	printf("All initialized.\n\n");

	printf("%d %d %d\n", bs_dimension, sdm_sample, sdm_radius);

	address = bs_init_random(bs_alloc());
	data = bs_init_random(bs_alloc());

	sdm_write(address, data);
	read = sdm_read(address);

	if (read != NULL) {
		bs_print(data);
		bs_print(read);
	}

	return 0;
}
