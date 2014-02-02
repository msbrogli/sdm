#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "memory.h"
#include "memory_thread.h"

#define min(a, b) ((a)<(b) ? (a) : (b))

extern unsigned int bs_len;
extern unsigned int bs_dimension;
extern unsigned int sdm_radius;

unsigned int sdm_thread_count = 4;

void sdm_thread_offset(sdm_thread_params* params, unsigned int* id, unsigned int* offset, unsigned int* len) {
	int qty, extra;
	qty = (params->sdm->sample)/sdm_thread_count;
	extra = (params->sdm->sample)%sdm_thread_count;
	*id = params->id;
	*len = qty + (*id < extra ? 1 : 0);
	*offset = (*id)*qty + min(*id, extra);
	//printf("@@ (%d) %d [%d, %d]\n", myid, len, offset, offset+len-1);
}

void* sdm_thread_radius_count_task(void* ptr) {
	sdm_thread_params* params = (sdm_thread_params*) ptr;
	unsigned int myid, offset, len;
	unsigned int i, counter = 0;
	unsigned int dist;

	sdm_thread_offset(params, &myid, &offset, &len);

	for(i=0; i<len; i++) {
		dist = bs_distance(get_sdm_memory_address(params->sdm, offset+i), params->address);
		if (dist <= params->radius) {
			counter++;
		}
	}

	params->counter = counter;
	return NULL;
}

unsigned int sdm_thread_radius_count(struct sdm_memory* sdm, bitstring* address, unsigned int radius) {
	pthread_t thread[sdm_thread_count];
	sdm_thread_params params[sdm_thread_count];
	unsigned int i, counter;
	for(i=0; i<sdm_thread_count; i++) {
		params[i].id = i;
		params[i].sdm = sdm;
		params[i].address = address;
		params[i].radius = radius;
		pthread_create(&thread[i], NULL, sdm_thread_radius_count_task, (void*) &params[i]);
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

void* sdm_thread_read_task(void* ptr) {
	sdm_thread_params* params = (sdm_thread_params*) ptr;
	unsigned int myid, offset, len;
	unsigned int i, j, counter = 0;
	unsigned int dist;

	sdm_thread_offset(params, &myid, &offset, &len);

	for(i=0; i<bs_dimension; i++) params->adder[i] = 0;
	for(i=0; i<len; i++) {
		dist = bs_distance(get_sdm_memory_address(params->sdm, offset+i), params->address);
		if (dist <= sdm_radius) {
			for(j=0; j<bs_dimension; j++) {
				params->adder[j] += get_sdm_memory_adder(params->sdm, offset+i, j);
			}
			counter++;
		}
	}

	params->counter = counter;
	return NULL;
}

bitstring* sdm_thread_read(struct sdm_memory* sdm, bitstring* address) {
	pthread_t thread[sdm_thread_count];
	sdm_thread_params params[sdm_thread_count];
	int32_t adder[sdm_thread_count][bs_dimension];
	int32_t adder2[bs_dimension];
	adder_t adder3[bs_dimension];
	unsigned int i, j;
	for(i=0; i<sdm_thread_count; i++) {
		params[i].id = i;
		params[i].sdm = sdm;
		params[i].address = address;
		params[i].adder = adder[i];
		pthread_create(&thread[i], NULL, sdm_thread_read_task, (void*) &params[i]);
	}
	for(i=0; i<sdm_thread_count; i++) {
		pthread_join(thread[i], NULL);
	}
	// clear accumulator
	for(j=0; j<bs_dimension; j++) adder2[j] = 0;
	// accumulate
	for(i=0; i<sdm_thread_count; i++) {
		for(j=0; j<bs_dimension; j++) {
			adder2[j] += adder[i][j];
		}
	}
	// we can't add all adders in an adder_t type because
	// it will probably overflow.
	for(j=0; j<bs_dimension; j++) {
		if (adder2[j] > 0) adder3[j] = 1;
		else if (adder2[j] < 0) adder3[j] = -1;
		else adder3[j] = (rand()%2 == 0 ? 1 : -1);
	}
	//printf("Hardlocation inside radius %d = %d\n", sdm_radius, counter);
	return bs_init_adder(bs_alloc(), adder3);
}

void* sdm_thread_radius_count_intersect_task(void* ptr) {
	sdm_thread_params* params = (sdm_thread_params*) ptr;
	unsigned int myid, offset, len;
	unsigned int i, counter = 0;
	unsigned int d1, d2;

	sdm_thread_offset(params, &myid, &offset, &len);

	for(i=0; i<len; i++) {
		d1 = bs_distance(get_sdm_memory_address(params->sdm, offset+i), params->addr1);
		d2 = bs_distance(get_sdm_memory_address(params->sdm, offset+i), params->addr2);
		if (d1 <= params->radius && d2 <= params->radius) {
			counter++;
		}
	}

	params->counter = counter;
	return NULL;
}

unsigned int sdm_thread_radius_count_intersect(struct sdm_memory* sdm, bitstring* addr1, bitstring* addr2, unsigned int radius) {
	pthread_t thread[sdm_thread_count];
	sdm_thread_params params[sdm_thread_count];
	unsigned int i, counter;
	for(i=0; i<sdm_thread_count; i++) {
		params[i].id = i;
		params[i].sdm = sdm;
		params[i].addr1 = addr1;
		params[i].addr2 = addr2;
		params[i].radius = radius;
		pthread_create(&thread[i], NULL, sdm_thread_radius_count_intersect_task, (void*) &params[i]);
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

void hl_write(adder_t* adder, bitstring* data) {
    int i, a;
    for(i=0; i<bs_dimension; i++) {
        a = bs_bitsign(data, i);
        if (a > 0) {
            if (adder[i] < 127) adder[i]++;
            else printf("@@ WARNING WARNING!\n");
        } else if (a < 0) {
            if (adder[i] > -127) adder[i]--;
            else printf("@@ WARNING WARNING!\n");
        }
    }
}

void* sdm_thread_write_task(void* ptr) {
	sdm_thread_params* params = (sdm_thread_params*) ptr;
	unsigned int myid, offset, len;
	unsigned int i, counter = 0;
	unsigned int dist;

	sdm_thread_offset(params, &myid, &offset, &len);

	for(i=0; i<len; i++) {
		dist = bs_distance(get_sdm_memory_address(params->sdm, offset+i), params->address);
		if (dist <= sdm_radius) {
			hl_write((adder_t*)&get_sdm_memory_adder(params->sdm, offset+i, 0), params->data);
			counter++;
		}
	}

	params->counter = counter;
	return NULL;
}

unsigned int sdm_thread_write(struct sdm_memory* sdm, bitstring* address, bitstring* data) {
	pthread_t thread[sdm_thread_count];
	sdm_thread_params params[sdm_thread_count];
	unsigned int i, counter;
	for(i=0; i<sdm_thread_count; i++) {
		params[i].id = i;
		params[i].sdm = sdm;
		params[i].address = address;
		params[i].data = data;
		pthread_create(&thread[i], NULL, sdm_thread_write_task, (void*) &params[i]);
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

void* sdm_thread_distance_task(void* ptr) {
	sdm_thread_params* params = (sdm_thread_params*) ptr;
	unsigned int myid, offset, len;
	unsigned int i;

	sdm_thread_offset(params, &myid, &offset, &len);

	for(i=0; i<len; i++) {
		params->res[offset+i] = bs_distance(get_sdm_memory_address(params->sdm, offset+i), params->address);
	}

	return NULL;
}

void sdm_thread_distance(struct sdm_memory* sdm, bitstring* address, unsigned int *res) {
	pthread_t thread[sdm_thread_count];
	sdm_thread_params params[sdm_thread_count];
	unsigned int i;
	for(i=0; i<sdm_thread_count; i++) {
		params[i].id = i;
		params[i].sdm = sdm;
		params[i].address = address;
		params[i].res = res;
		pthread_create(&thread[i], NULL, sdm_thread_distance_task, (void*) &params[i]);
	}
	for(i=0; i<sdm_thread_count; i++) {
		pthread_join(thread[i], NULL);
	}
}

