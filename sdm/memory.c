#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "memory.h"
#include "hardlocation.h"
#include "bitstring.h"

extern unsigned int bs_dimension;

// Number of samples from space.
// It is equivalent to |N'|, in Kanerva's book.
unsigned int sdm_sample = 1000000;

unsigned int sdm_radius = 447;

hardlocation** sdm_memory;

int sdm_initialize() {
	unsigned int i;

	bs_initialize();
	hl_initialize();

	sdm_memory = (hardlocation**) malloc(sizeof(hardlocation*)*sdm_sample);
	assert(sdm_memory != NULL);

	for(i=0; i<sdm_sample; i++) {
		sdm_memory[i] = hl_init_random(hl_alloc());
	}

	return 0;
}

void sdm_free() {
	unsigned int i;

	for(i=0; i<sdm_sample; i++) {
		hl_free(sdm_memory[i]);
	}

	free(sdm_memory);
}

int sdm_write(bitstring* address, bitstring* data) {
	unsigned int i, j, counter = 0;
	unsigned int dist;
	for(i=0; i<sdm_sample; i++) {
		dist = bs_distance(sdm_memory[i]->address, address);
		if (dist <= sdm_radius) {
			for(j=0; j<bs_dimension; j++) {
				sdm_memory[i]->adder[j] += bs_bitsign(data, j);
			}
			counter++;
		}
	}
	printf("Hardlocations inside radius %d = %d\n", sdm_radius, counter);
	return 0;
}

bitstring* sdm_read(bitstring* address) {
	unsigned int i, j, counter = 0;
	unsigned int dist;
	adder_t adder[bs_dimension];
	memset(adder, 0, sizeof(adder));
	for(i=0; i<sdm_sample; i++) {
		dist = bs_distance(sdm_memory[i]->address, address);
		if (dist <= sdm_radius) {
			for(j=0; j<bs_dimension; j++) {
				adder[j] += sdm_memory[i]->adder[j];
			}
			counter++;
		}
	}
	printf("Hardlocation inside radius %d = %d\n", sdm_radius, counter);
	return bs_init_adder(bs_alloc(), adder);
}

