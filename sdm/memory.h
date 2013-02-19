#ifndef SDM_MEMORY_H_
#define SDM_MEMORY_H_

#include <stdint.h>

#include "common.h"
#include "bitstring.h"

#define get_sdm_memory_address(i) (&sdm_memory_address[(i)*bs_len])
#define get_sdm_memory_adder(i, j) (sdm_memory_adder[(i)*bs_dimension+(j)])

// TODO comment
int sdm_initialize();
int sdm_initialize_from_file(char* filename);
void sdm_free();

int sdm_save_to_file(char* filename);
int sdm_load_from_file(char* filename);

#endif
