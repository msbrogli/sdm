#ifndef SDM_BITSTRING_H_
#define SDM_BITSTRING_H_

#include <stdint.h>

#include "common.h"

//#define BS_DEBUG

// bs_table size is 2^(BS_TABLE_SIZE)
#define BS_TABLE_SIZE 16

// Initilize bitstring library.
// Must be called before using any other bitstring function.
int bs_initialize();

// Alloc bitstring without initializing it.
// Must be called before any bs_init_* function.
// Sample usage:
//     bitstring* a = bs_alloc();
bitstring* bs_alloc();

// Free bitstring memory.
void bs_free(bitstring* a);

// Initialize a bitstring already allocated.
// Must call bs_alloc before use any bs_init_* function.
// Sample usage:
//     bitstring* a = bs_init_random(bs_alloc());
//
// bs_init_zero initializes to zero.
// bs_init_random initializes using rand() from stdlib.
bitstring* bs_init_random(bitstring* a);
bitstring* bs_init_zero(bitstring* a);
bitstring* bs_init_adder(bitstring* a, adder_t *adder);

// Alloc and copy any bitstring content.
// Must NOT call bs_alloc before.
// Sample usage:
//     bitstring* a, b;
//     a = bs_init_random(bs_alloc());
//     b = bs_copy(a);
bitstring* bs_copy(bitstring* a);

// Calculates distance between two bitstring.
// Both bitstring must have same dimensions.
// Sample usage:
//     int d;
//     bitstring* a, b;
//     a = bs_init_random(bs_alloc());
//     b = bs_copy(a);
//     b[0] ^= 0x1248f;      // swap 8 bits
//     d = bs_distance(a, b);    // d = 8
unsigned int bs_distance(bitstring* a, bitstring* b);

// TODO Comment.
int bs_bitsign(bitstring* a, int bit);
void bs_bitset(bitstring* a, int bit);
void bs_bitclear(bitstring* a, int bit);
void bs_print(bitstring* a);
void bs_string(bitstring* a, char* str);
void bs_bitswap(bitstring* a, int bit);
bitstring* bs_average(bitstring* a, bitstring* b);
void bs_bitrandomswap(bitstring* a, int qty);

#endif
