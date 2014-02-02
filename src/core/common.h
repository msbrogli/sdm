#ifndef SDM_COMMON_H_
#define SDM_COMMON_H_

typedef uint64_t bitstring;
typedef int8_t adder_t;

struct sdm_memory {

	// Pointer to an array of bitstrings.
	// The ith bitstring if the address of the ith hard-location.
	bitstring* addresses;

	// Pointer to an array of adders.
	// The ith adder belongs to the ith hard-location.
	adder_t* adders;

	// Number of hard-locations, that is, number of samples from the whole space {0,1}^n.
	// It is equivalent to |N'| in Kanerva's book.
	unsigned int sample;

};

#endif
