#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memory.h"

// Number of samples from space.
// It is equivalent to |N'|, in Kanerva's book.
const unsigned int sdm_sample = 1000000;

unsigned int sdm_radius = 437;

hardlocation** sdm_memory;

int sdm_initialize() {
	unsigned int i;

	sdm_memory = (hardlocation**) malloc(sizeof(hardlocation*)*sdm_sample);
	assert(sdm_memory != NULL);

	for(i=0; i<sdm_sample; i++) {
		sdm_memory[i] = hl_init_random(hl_alloc());
	}

	return 0;
}

int sdm_write(bitstring* address, bitstring* data) {
	unsigned int i, counter = 0;
	unsigned int dist;
	for(i=0; i<sdm_sample; i++) {
		dist = bs_distance(sdm_memory[i]->address, address);
		if (dist <= sdm_radius) {
			counter++;
		}
	}
	printf("Hardlocations inside radius %d = %d\n", sdm_radius, counter);
	return 0;
}

bitstring* sdm_read(bitstring* address) {
	return NULL;
}
