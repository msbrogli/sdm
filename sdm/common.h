#ifndef SDM_COMMON_H_
#define SDM_COMMON_H_

typedef uint64_t bitstring;

typedef int16_t adder_t;

typedef struct {
	bitstring* address;
	adder_t* adder;
	int num_items;
	float p;
} hardlocation;


#endif
