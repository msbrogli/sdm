#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "memory.h"
#include "bitstring.h"

extern unsigned int bs_dimension;
extern unsigned int bs_len;

// Number of samples from space.
// It is equivalent to |N'|, in Kanerva's book.
unsigned int sdm_sample = 1000000;

unsigned int sdm_radius = 451;

const char sdm_signature[4] = "SDM";
const char sdm_version[4] = "0.1";

bitstring* sdm_memory_address;
adder_t* sdm_memory_adder;

/*
int sdm_save_to_file(char* filename) {
	FILE* fp;
	unsigned int i;
	size_t szbitstring, szadder_t;

	fp = fopen(filename, "w");
	if (fp == NULL ) return -1;

	szbitstring = sizeof(bitstring);
	szadder_t = sizeof(adder_t);

	fputs(sdm_signature, fp);
	fputs(sdm_version, fp);
	fwrite(&bs_dimension, sizeof(bs_dimension), 1, fp);
	fwrite(&sdm_sample, sizeof(sdm_sample), 1, fp);
	fwrite(&szbitstring, sizeof(&szbitstring), 1, fp);
	fwrite(&szadder_t, sizeof(&szadder_t), 1, fp);

	for(i=0; i<sdm_sample; i++) {
		fwrite(sdm_memory[i]->address, sizeof(bitstring), bs_len, fp);
		fwrite(sdm_memory[i]->adder, sizeof(adder_t), bs_dimension, fp);
	}

	fclose(fp);
	return 0;
}

int sdm_load_from_file(char* filename) {
	FILE* fp;
	unsigned int i;
	unsigned int dimension, sample;
	size_t bzbitstring, bzadder_t;
	char signature[4], version[4];
	char c;

	fp = fopen(filename, "r");
	if (fp == NULL) return -1;

	fread(signature, sizeof(char), 3, fp);
	signature[3] = '\0';
	if (strcmp(signature, sdm_signature)) return -2;

	fread(version, sizeof(char), 3, fp);
	version[3] = '\0';
	if (strcmp(version, sdm_version)) return -3;

	fread(&dimension, sizeof(dimension), 1, fp);
	if (dimension != bs_dimension) return -4;

	fread(&sample, sizeof(sample), 1, fp);
	if (sample != sdm_sample) return -5;

	fread(&bzbitstring, sizeof(bzbitstring), 1, fp);
	if (bzbitstring != sizeof(bitstring)) return -6;

	fread(&bzadder_t, sizeof(bzadder_t), 1, fp);
	if (bzadder_t != sizeof(adder_t)) return -7;

	for(i=0; i<sdm_sample; i++) {
		sdm_memory[i] = hl_alloc();
		fread(sdm_memory[i]->address, sizeof(bitstring), bs_len, fp);
		fread(sdm_memory[i]->adder, sizeof(adder_t), bs_dimension, fp);
	}

	assert(fread(&c, sizeof(char), 1, fp) == 0);
	assert(feof(fp));

	fclose(fp);
	return 0;
}

int sdm_initialize_from_file(char* filename) {
	int ret;

	bs_initialize();
	hl_initialize();

	sdm_memory = (hardlocation**) malloc(sizeof(hardlocation*)*sdm_sample);
	assert(sdm_memory != NULL);

	ret = sdm_load_from_file(filename);
	if (ret != 0) {
		free(sdm_memory);
	}

	return ret;
}
*/

int sdm_initialize(struct sdm_memory *sdm) {
	unsigned int i, n, sample;
	sample = sdm->sample;

	bs_initialize();

	// Alloc the addresses.
	sdm->addresses = (bitstring*) malloc(sizeof(bitstring)*bs_len*sample);
	if (sdm->addresses == NULL) {
		return -1;
	}

	// Alloc the adders.
	n = bs_dimension * sample;
	sdm->adders = (adder_t*) malloc(sizeof(adder_t)*n);
	if (sdm->adders == NULL) {
		free(sdm->addresses);
		return -1;
	}

	// Reset the adders.
	bzero(sdm->adders, sizeof(adder_t)*n);

	// Generate random bitstring for the addresses of the hard-locations.
	for(i=0; i < sample; i++) {
		bs_init_random(get_sdm_memory_address(sdm, i));
	}

	return 0;
}

void sdm_free(struct sdm_memory* sdm) {
	free(sdm->addresses);
	free(sdm->adders);
}

