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

	//LINHARES START
	//hl->num_items = malloc(sizeof(int));
	hl->num_items = 0;
	
	float N = (float) bs_dimension;
	//hl->p = malloc(sizeof(float));										 
	hl->p = (N-451.0)/N;
	//LINHARES END
	
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







//LINHARES START
void hl_write_linhares(hardlocation* hl, bitstring* data) {
	int i, a;
	for(i=0; i<bs_dimension; i++) {
		//writing only the difference may give us more space 
		a = bs_bitsign(hl->address,i)^bs_bitsign(data, i);
		if (a > 0) {
			if (hl->adder[i] < 127) hl->adder[i]++;
			else printf("@@ WARNING WARNING!\n");
		} 
	}
	hl->num_items++;
}

bitstring* hl_read_linhares(hardlocation* hl) {
	//Computing the average
	//AVG= p * num_items
	float avg = hl->p * hl->num_items;
	
	bitstring* a = bs_init_adder_linhares (hl->address, hl->adder, avg);
	
	//If hl->adder[i] then AVG, select 1 otherwise, 0.
	
	return a;
}
//LINHARES END




// DO ANOTHER READ, probabilistically, using stdevs, from -3 to 3.















