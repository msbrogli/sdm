
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
	__global hardlocation** memory,
	__global bitstring* address,
	const unsigned int radius)
{
	int i = get_global_id(0);
	if (bs_distance_to(memory[i], address), radius) {
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

