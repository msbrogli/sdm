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
	assert(hl->adder != NULL);
	return hl;
}

hardlocation* hl_init_random(hardlocation* hl) {
	bs_init_random(hl->address);
	memset(hl->adder, 0, sizeof(hl->adder[0])*bs_dimension);
	return hl;
}

void hl_free(hardlocation* hl) {
	bs_free(hl->address);
	free(hl->adder);
	free(hl);
}

void hl_write(hardlocation* hl, bitstring* data) {
	int i, a;
	for(i=0; i<bs_dimension; i++) {
		a = bs_bitsign(data, i);
		if (a > 0) {
			if (hl->adder[i] < 127) hl->adder[i]++;
			else printf("@@ WARNING WARNING!\n");
		} else if (a < 0) {
			if (hl->adder[i] > -127) hl->adder[i]--;
			else printf("@@ WARNING WARNING!\n");
		}
	}
}

bitstring* hl_read(hardlocation* hl) {
	return bs_init_adder(bs_alloc(), hl->adder);
}

