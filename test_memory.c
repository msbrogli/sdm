#include <stdio.h>
#include "sdm/memory.h"

int main(void) {
	bitstring* address;
	bitstring* data;

	bs_initialize();
	hl_initialize();
	sdm_initialize();

	printf("All initialized.\n\n");

	address = bs_init_random(bs_alloc());
	data = bs_init_random(bs_alloc());
	sdm_write(address, data);

	printf("Working!\n");

	return 0;
}
