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
	assert(RAND_MAX == 0x7fffffff);

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

uint32_t myrand() {
	uint32_t a = rand();
	if (rand()%2 == 0) a |= 0x80000000;
	return a;
}

bitstring* bs_init_random(bitstring* a) {
	unsigned int i, d;
	for(i=0; i<bs_len; i++) {
		//a[i] = ((uint64_t)(rand()&0xffff))<<48 | ((uint64_t)(rand()&0xffff))<<32 | (rand()&0xffff)<<16 | (rand()&0xffff);
		//a[i] = ((uint64_t)rand())<<32 | rand();
		a[i] = ((uint64_t)myrand())<<32 | myrand();
	}
	// clear lasts bits to fit bs_dimension
	d = 8*bs_len*sizeof(bitstring) - bs_dimension;
	for(i=0; i<d; i++) {
		a[0] &= ~((uint64_t)1<<(63-i));
	}
	return a;
}

bitstring* bs_init_adder(bitstring* a, adder_t *adder) {
	unsigned int i;
	bs_init_random(a);
	for(i=0; i<bs_dimension; i++) {
		if(adder[i] > 0) bs_bitset(a, i);
		else if(adder[i] < 0) bs_bitclear(a, i);
	}
	return a;
}

unsigned int bs_distance(bitstring* a, bitstring* b) {
	register unsigned int i;
	//register unsigned int j;
	unsigned int dist;
	uint64_t c;
	//const unsigned int n = 8*sizeof(a[0])/BS_TABLE_SIZE;

	dist = 0;
	for(i=0; i<bs_len; i++) {
		c = a[i] ^ b[i];

		// optimizing!
		// take care when chaning BS_TABLE_SIZE or bitstring!
		// 16 bits table
		//dist += bs_table[c&0xffff] + bs_table[(c>>16)&0xffff] + bs_table[(c>>32)&0xffff] + bs_table[(c>>48)&0xffff];
		// 8 bits table
		dist += bs_table[c&0xff] + bs_table[(c>>8)&0xff] + bs_table[(c>>16)&0xff] + bs_table[(c>>24)&0xff] + 
				bs_table[(c>>32)&0xff] + bs_table[(c>>40)&0xff] + bs_table[(c>>48)&0xff] + bs_table[(c>>56)&0xff];

		//for(j=0; j<n; j++) {
		//	dist += bs_table[(c>>(j*BS_TABLE_SIZE))&0xff];
		//}
	}
	return dist;
}

// TODO Make it an inline function. [msbrogli 2014-02-01]
int bs_bit(bitstring* a, int bit) {
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

void bs_bitswap(bitstring* a, int bit) {
	int i = bit/64, j = bit%64;
	a[bs_len-1-i] ^= ((uint64_t)1<<j);
}

void bs_string(bitstring* a, char* str) {
	int i;
	for(i=0; i<bs_len; i++) {
		sprintf(str+16*i, "%016llx", a[i]);
	}
}

void bs_bitrandomswap(bitstring* a, int qty) {
	unsigned int i, x, counter, arr[qty];
	counter = 0;
	while(counter < qty) {
		x = rand() % bs_dimension;
		for(i=0; i<counter; i++) {
			if (arr[i] == x) break;
		}
		if (i == counter) {
			arr[counter] = x;
			counter++;
		}
	}
	for(i=0; i<qty; i++) {
		bs_bitswap(a, arr[i]);
	}
}

bitstring* bs_average(bitstring* a, bitstring* b) {
	bitstring* avg = bs_alloc();
	unsigned int i;
	uint64_t c, p;
	int first = 1;
	for(i=0; i<bs_len; i++) {
		avg[i] = a[i];
		c = a[i]^b[i];
		p = 1;
		while(c) {
			if (c&1) {
				if (first) avg[i] ^= p;
				first = 1 - first;
			}
			p <<= 1;
			c >>= 1;
		}
	}
	return avg;
}

void bs_print(bitstring* a) {
	int i;
	printf("0x");
	for(i=0; i<bs_len; i++) {
		printf("%016llx", a[i]);
	}
	printf("\n");
}

