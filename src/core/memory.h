#ifndef SDM_MEMORY_H_
#define SDM_MEMORY_H_

#include <stdint.h>

#include "common.h"
#include "bitstring.h"

#define get_sdm_memory_address(sdm, i) (&(sdm->addresses[(i)*bs_len]))
#define get_sdm_memory_adder(sdm, i, j) (sdm->adders[(i)*(bs_dimension)+(j)])

// TODO comment
int sdm_initialize(struct sdm_memory* sdm);
int sdm_initialize_from_file(char* filename);
void sdm_free(struct sdm_memory* sdm);

int sdm_save_to_file(char* filename);
int sdm_load_from_file(char* filename);

#endif
