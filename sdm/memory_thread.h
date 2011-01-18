#ifndef SDM_MEMORY_THREAD_H_
#define SDM_MEMORY_THREAD_H_

#include <stdint.h>

#include "common.h"
#include "bitstring.h"
#include "hardlocation.h"

typedef struct {
	int id;
	bitstring* address;
	bitstring* addr1;
	bitstring* addr2;
	bitstring* data;
	unsigned int radius;
	unsigned int counter;
	unsigned int *res;
	int32_t *adder;
} sdm_thread_params;

// TODO comment
unsigned int sdm_thread_radius_count(bitstring* address, unsigned int radius);
unsigned int sdm_thread_radius_count_intersect(bitstring* addr1, bitstring* addr2, unsigned int radius);
unsigned int sdm_thread_write(bitstring* address, bitstring* data);
bitstring* sdm_thread_read(bitstring* address);
void sdm_thread_distance(bitstring* address, unsigned int *res);

#endif
