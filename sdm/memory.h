#ifndef SDM_MEMORY_H_
#define SDM_MEMORY_H_

#include <stdint.h>

#include "bitstring.h"
#include "hardlocation.h"

int sdm_initialize();
int sdm_write(bitstring* address, bitstring* data);
bitstring* sdm_read(bitstring* address);

#endif
