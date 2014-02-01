#include <stdio.h>
#include <stdlib.h>
#include "sdm/memory.h"

extern unsigned int bs_dimension;
extern unsigned int sdm_sample;
extern unsigned int sdm_radius;

int main(int argv, char **argc) {
	bitstring* address;
	bitstring* data;
	int i, lo, hi;

	if (argv == 4) {
		bs_dimension = atoi(argc[1]);
		sdm_sample = atoi(argc[2]);
		sdm_radius = atoi(argc[3]);
	}

	bs_initialize();
	hl_initialize();
	sdm_initialize();

	//printf("All initialized.\n\n");

	printf("%d %d %d\n", bs_dimension, sdm_sample, sdm_radius);

	address = bs_init_random(bs_alloc());
	data = bs_init_random(bs_alloc());

	lo = 0;
	hi = bs_dimension;
	for(i=lo; i<=hi; i++) {
		sdm_radius = i;
		sdm_write(address, data);
		fflush(stdout);
	}

	return 0;
}
