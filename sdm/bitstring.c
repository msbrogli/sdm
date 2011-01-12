#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "bitstring.h"

// Number of dimension in our memory space.
unsigned int bs_dimension = 1000;

// Number of steps when iterating over bitstring.
unsigned int bs_len;

// Table used to accelerate counting bitstring's bits 1.
// It is filled during bs_initialize.
unsigned int bs_table[1<<BS_TABLE_SIZE];

#ifdef BS_DEBUG
#define bs_debug printf
#else
void bs_debug(char *buf, ...) {}
#endif

#ifdef BS_TRACE
#define bs_trace printf
#else
void bs_trace(char *buf, ...) {}
#endif

int bs_initialize() {
	int i, j, n;

	// initilize bs_len
	bs_debug("bs_dimension=%d\n", bs_dimension);
	bs_debug("bits of each bitstring=%d\n", 8*(int)sizeof(bitstring));
	bs_len = bs_dimension/8/sizeof(bitstring);
	if (8*bs_len*sizeof(bitstring) < bs_dimension) {
		bs_len++;
	}
	bs_debug("bs_len=%d\n", bs_len);

	// initialize bs_table
	n = 1<<BS_TABLE_SIZE;
	for(i=0; i<n; i++) {
		bs_table[i] = 0;
		j = i;
		while(j) {
			bs_table[i] += j&1;
			j >>= 1;
		}
	}

	// init random for bs_init_random
	srand(time(NULL));

	bs_debug("bitstring initialized successfully!\n");
	return 0;
}

bitstring* bs_alloc() {
	bitstring* a = (bitstring*) malloc(sizeof(a[0])*bs_len);
	assert(a != NULL);
	return a;
}

void bs_free(bitstring* a) {
	free(a);
}

bitstring* bs_copy(bitstring* a) {
	unsigned int i;
	bitstring* b = bs_alloc();
	for(i=0; i<bs_len; i++) {
		b[i] = a[i];
	}
	return b;
}

bitstring* bs_init_zero(bitstring* a) {
	unsigned int i;
	for(i=0; i<bs_len; i++) {
		a[i] = 0;
	}
	return a;
}

bitstring* bs_init_random(bitstring* a) {
	// FIXME clear lasts bits to fit bs_dimension
	unsigned int i;
	for(i=0; i<bs_len; i++) {
		a[i] = ((uint64_t)rand())<<32 | rand();
	}
	return a;
}

bitstring* bs_init_adder(bitstring* a, adder_t *adder) {
	unsigned int i;
	for(i=0; i<bs_dimension; i++) {
		if(adder[i] >= 0) bs_bitset(a, i);
		else bs_bitclear(a, i);
	}
	return a;
}

unsigned int bs_distance(bitstring* a, bitstring* b) {
	unsigned int i, j, n, dist;
	uint64_t c;
	uint16_t* p;

	p = (uint16_t*)&c;
	n = sizeof(a[0])/sizeof(p[0]);
	dist = 0;
	bs_trace("bs_distance loop\n");
	for(i=0; i<bs_len; i++) {
		c = a[i] ^ b[i];
		bs_trace("  %llx ^ %llx = %llx\n", a[i], b[i], c);
		for(j=0; j<n; j++) {
			dist += bs_table[p[j]];
		}
	}
	bs_trace("bs_distance = %d\n", dist);
	return dist;
}

inline int bs_bit(bitstring* a, int bit) {
	int i = bit/64, j = bit%64;
	return (a[bs_len-1-i]&((uint64_t)1<<j) ? 1 : 0);
}

int bs_bitsign(bitstring* a, int bit) {
	return (bs_bit(a, bit) ? 1 : -1);
}

void bs_bitset(bitstring* a, int bit) {
	int i = bit/64, j = bit%64;
	a[bs_len-1-i] |= ((uint64_t)1<<j);
}

void bs_bitclear(bitstring* a, int bit) {
	int i = bit/64, j = bit%64;
	a[bs_len-1-i] &= ~((uint64_t)1<<j);
}

void bs_print(bitstring* a) {
	int i;
	printf("0x");
	for(i=0; i<bs_len; i++) {
		printf("%llx", a[i]);
	}
	printf("\n");
}

