#ifndef SDM_MEMORY_THREAD_H_
#define SDM_MEMORY_THREAD_H_

#include <stdint.h>

#include "common.h"
#include "bitstring.h"

typedef struct {
	int id;
	struct sdm_memory* sdm;
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
unsigned int sdm_thread_radius_count(struct sdm_memory* sdm, bitstring* address, unsigned int radius);
unsigned int sdm_thread_radius_count_intersect(struct sdm_memory* sdm, bitstring* addr1, bitstring* addr2, unsigned int radius);
unsigned int sdm_thread_write(struct sdm_memory* sdm, bitstring* address, bitstring* data);
bitstring* sdm_thread_read(struct sdm_memory* sdm, bitstring* address);
void sdm_thread_distance(struct sdm_memory* sdm, bitstring* address, unsigned int *res);

#endif
