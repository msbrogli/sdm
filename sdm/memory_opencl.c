#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <OpenCL/opencl.h>

#include "memory.h"
#include "memory_opencl.h"
#include "hardlocation.h"
#include "bitstring.h"

const char *testKernelSource = "\n" \
"__kernel square(                                                       \n" \
"   __global float* input,                                              \n" \
"   __global float* output,                                             \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   if(i < count)                                                       \n" \
"       output[i] = input[i] * input[i];                                \n" \
"}                                                                      \n" \
"\n";

unsigned int sdm_opencl_radius_count(bitstring* address, unsigned int radius) {
	int err;
	size_t len;
	char buffer[2048];

	char kernelBuffer[4096];
	char *kernelSource = kernelBuffer;

	cl_device_id device_id;
	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel kernel;

	FILE *fp;

	fp = fopen("../sdm/memory_opencl_program.cl", "r");
	assert(fp);
	len = fread((void*)kernelBuffer, sizeof(char), sizeof(kernelBuffer)-1, fp);
	kernelSource[len] = '\0';
	fclose(fp);

	err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	assert(err == CL_SUCCESS);

	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	assert(context);

	commands = clCreateCommandQueue(context, device_id, 0, &err);
	assert(commands);

	program = clCreateProgramWithSource(context, 1, (const char**) &kernelSource, NULL, &err);
	assert(program);

	err = clBuildProgram(program, 0, NULL, "-I../sdm", NULL, NULL);
	if (err != CL_SUCCESS) {
		fprintf(stderr, "Error: Failed to build program executable.\n");
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		fprintf(stderr, "%s\n", buffer);
		assert(err == CL_SUCCESS);
	}

	kernel = clCreateKernel(program, "count", &err);
	assert(kernel && err == CL_SUCCESS);

	return 0;
}

