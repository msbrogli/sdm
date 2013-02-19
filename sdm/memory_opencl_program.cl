
// Quick reference guide: http://www.khronos.org/files/opencl-quick-reference-card.pdf

// According to quick reference, unsigned long has 64-bit unsigned.
typedef unsigned long uint64_t;

// According to quick reference, unsigned int has 32-bit unsigned.
typedef unsigned int uint32_t;

// According to quick reference, unsigned short has 16-bit unsigned.
typedef unsigned short uint16_t;

// According to quick reference, unsigned char has 8-bit unsigned.
typedef unsigned char uint8_t;

typedef uint64_t bitstring;
typedef uint8_t adder_t;

typedef struct {
	bitstring* address;
	adder_t* adder;
} hardlocation;

// Doesn't have enough constant memory for 16 bits
#define BS_TABLE_SIZE 8

__constant int bs_table[1<<BS_TABLE_SIZE];
__constant unsigned int bs_dimension = 1000;
__constant unsigned int bs_len = 16;

unsigned int bs_distance(
	__global bitstring* a, 
	__global bitstring* b) 
{
	unsigned int i, dist;
	uint64_t c;
	dist = 0;
	for(i=0; i<bs_len; i++) {
		//c = a[i] ^ b[i];
		c = b[i];
		while(c) {
			if(c&1) dist++;
			c >>= 1;
		}
		//dist += bs_table[c&0xff] + bs_table[(c>>8)&0xff] + bs_table[(c>>16)&0xff] + bs_table[(c>>24)&0xff] +
		//        bs_table[(c>>32)&0xff] + bs_table[(c>>40)&0xff] + bs_table[(c>>48)&0xff] + bs_table[(c>>56)&0xff];
	}
	return dist;
}


///probably good to make a method returning a pointer to the list of of a bitstring's hard locations 




__kernel void
write(
	__global hardlocation** memory,
	__global bitstring* address,
	__global bitstring* data,
	const unsigned int radius)
{
}

__kernel void
read(
	__global hardlocation** memory,
	__global bitstring* address,
	const unsigned int radius)
{
}

__kernel void
count(
	hardlocation** memory,
	bitstring* address,
	int* counter,
	const unsigned int radius)
{
	int id = get_global_id(0);
	if (bs_distance(memory[id]->address, address) <= radius) {
		atom_add(counter, 1);
	}
}

__kernel void
intersect(
	__global hardlocation** memory,
	__global bitstring* addr1,
	__global bitstring* addr2,
	const unsigned int radius)
{
}

