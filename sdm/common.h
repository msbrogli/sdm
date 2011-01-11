#ifndef SDM_COMMON_H_
#define SDM_COMMON_H_

typedef uint64_t bitstring;

typedef int8_t adder_t;

typedef struct {
	bitstring* address;
	adder_t* adder;
} hardlocation;


#endif
