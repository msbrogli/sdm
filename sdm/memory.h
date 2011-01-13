#ifndef SDM_MEMORY_H_
#define SDM_MEMORY_H_

#include <stdint.h>

#include "common.h"
#include "bitstring.h"
#include "hardlocation.h"

// TODO comment
int sdm_initialize();
void sdm_free();
unsigned int sdm_radius_count(bitstring* address, unsigned int radius);
unsigned int sdm_write(bitstring* address, bitstring* data);
bitstring* sdm_read(bitstring* address);

#endif
