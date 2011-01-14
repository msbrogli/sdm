#ifndef SDM_MEMORY_H_
#define SDM_MEMORY_H_

#include <stdint.h>

#include "common.h"
#include "bitstring.h"
#include "hardlocation.h"

typedef struct {
	int id;
	bitstring* address;
	unsigned int counter;
} sdm_find_near_params;

// TODO comment
int sdm_initialize();
void sdm_free();
unsigned int sdm_radius_count(bitstring* address, unsigned int radius);
unsigned int sdm_radius_count_intersect(bitstring* addr1, bitstring* addr2, unsigned int radius);
unsigned int sdm_write(bitstring* address, bitstring* data);
bitstring* sdm_read(bitstring* address);
void sdm_distance(bitstring* address, unsigned int *res);

#endif
