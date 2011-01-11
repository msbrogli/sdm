#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hardlocation.h"
#include "bitstring.h"

extern const unsigned int bs_dimension;

int hl_initialize() {
	return 0;
}

hardlocation* hl_alloc() {
	hardlocation* hl = (hardlocation*) malloc(sizeof(hardlocation));
	assert(hl != NULL);
	hl->address = bs_alloc();
	hl->adder = (typeof(hl->adder)) malloc(sizeof(hl->adder[0])*bs_dimension);
	return hl;
}

hardlocation* hl_init_random(hardlocation* hl) {
	bs_init_random(hl->address);
	memset(hl->adder, 0, sizeof(hl->adder[0])*bs_dimension);
	return hl;
}

void hl_free(hardlocation* hl) {
	bs_free(hl->address);
	free(hl);
}

void hl_write(hardlocation* hl, bitstring* data) {
}

bitstring* hl_read(hardlocation* hl) {
	return NULL;
}

