#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "memory.h"
#include "hardlocation.h"
#include "bitstring.h"

#define min(a, b) ((a)<(b) ? (a) : (b))

extern unsigned int bs_dimension;

// Number of samples from space.
// It is equivalent to |N'|, in Kanerva's book.
unsigned int sdm_sample = 1000000;

unsigned int sdm_radius = 451;

unsigned int sdm_thread_count = 4;

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

unsigned int sdm_radius_count(bitstring* address, unsigned int radius) {
	unsigned int i, counter = 0;
	unsigned int dist;
	for(i=0; i<sdm_sample; i++) {
		dist = bs_distance(sdm_memory[i]->address, address);
		if (dist <= radius) {
			counter++;
		}
	}
	//printf("Hardlocations inside radius %d = %d\n", sdm_radius, counter);
	return counter;
}

unsigned int sdm_radius_count_intersect(bitstring* addr1, bitstring* addr2, unsigned int radius) {
	unsigned int i, counter = 0;
	unsigned int d1, d2;
	for(i=0; i<sdm_sample; i++) {
		d1 = bs_distance(sdm_memory[i]->address, addr1);
		d2 = bs_distance(sdm_memory[i]->address, addr2);
		if (d1 <= radius && d2 <= radius) {
			counter++;
		}
	}
	return counter;
}

unsigned int sdm_write(bitstring* address, bitstring* data) {
	unsigned int i, counter = 0;
	unsigned int dist;
	for(i=0; i<sdm_sample; i++) {
		dist = bs_distance(sdm_memory[i]->address, address);
		if (dist <= sdm_radius) {
			hl_write(sdm_memory[i], data);
			counter++;
		}
	}
	//printf("Hardlocations inside radius %d = %d\n", sdm_radius, counter);
	return counter;
}

bitstring* sdm_read(bitstring* address) {
	unsigned int i, j, k, counter = 0;
	unsigned int dist;
	int32_t adder[bs_dimension];
	adder_t adder2[bs_dimension];
	memset(adder, 0, sizeof(adder));
	for(i=0; i<sdm_sample; i++) {
		dist = bs_distance(sdm_memory[i]->address, address);
		if (dist <= sdm_radius) {
			for(j=0; j<bs_dimension; j++) {
				if (sdm_memory[i]->adder[j] > 0) k = 1;
				else if (sdm_memory[i]->adder[j] < 0) k = -1;
				else k = (rand()%2 == 0 ? 1 : -1);
				adder[j] += k;
			}
			counter++;
		}
	}
	// we can't add all adders in an adder_t type because
	// it will probably overflow.
	for(i=0; i<bs_dimension; i++) {
		if (adder[i] > 0) adder2[i] = 1;
		else if (adder[i] < 0) adder2[i] = -1;
		else adder2[i] = 0;
	}
	//printf("Hardlocation inside radius %d = %d\n", sdm_radius, counter);
	return bs_init_adder(bs_alloc(), adder2);
}

void sdm_distance(bitstring* address, unsigned int *res) {
	unsigned int i;
	for(i=0; i<sdm_sample; i++) {
		res[i] = bs_distance(sdm_memory[i]->address, address);
	}
}

typedef struct {
	int id;
	bitstring* address;
	unsigned int counter;
} sdm_find_near_params;

void* sdm_find_near_thread(void* ptr) {
	sdm_find_near_params* params = (sdm_find_near_params*) ptr;
	bitstring* address;
	unsigned int myid;
	unsigned int qty, extra;
	unsigned int offset, len;
	unsigned int i, counter = 0;
	unsigned int dist;

	myid = params->id;
	address = params->address;

	qty = sdm_sample/sdm_thread_count;
	extra = sdm_sample%sdm_thread_count;
	len = qty + (myid < extra ? 1 : 0);
	offset = myid*qty + min(myid, extra);
	//printf("@@ (%d) %d [%d, %d]\n", myid, len, offset, offset+len-1);

	for(i=0; i<len; i++) {
		dist = bs_distance(sdm_memory[offset+i]->address, address);
		if (dist <= sdm_radius) {
			counter++;
		}
	}

	params->counter = counter;
	return NULL;
}

unsigned int sdm_find_near(bitstring* address) {
	pthread_t thread[sdm_thread_count];
	sdm_find_near_params params[sdm_thread_count];
	int iret[sdm_thread_count];
	unsigned int i, counter;
	for(i=0; i<sdm_thread_count; i++) {
		params[i].id = i;
		params[i].address = address;
		iret[i] = pthread_create(&thread[i], NULL, sdm_find_near_thread, (void*) &params[i]);
	}
	for(i=0; i<sdm_thread_count; i++) {
		pthread_join(thread[i], NULL);
	}
	counter = 0;
	for(i=0; i<sdm_thread_count; i++) {
		counter += params[i].counter;
	}
	return counter;
}

