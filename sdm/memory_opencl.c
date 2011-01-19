#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <OpenCL/opencl.h>

#include "memory.h"
#include "memory_opencl.h"
#include "hardlocation.h"
#include "bitstring.h"

extern unsigned int sdm_sample;
extern hardlocation** sdm_memory;

extern unsigned int bs_len;

unsigned int sdm_opencl_radius_count(bitstring* address, unsigned int radius) {
	int err;
	size_t len;
	char buffer[2048];

	char *kernelSource;

	size_t global;
	size_t local;

	cl_device_id device_id;
	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel kernel;

	cl_mem in_memory;
	cl_mem in_address;
	cl_mem in_counter;

	unsigned int counter;

	FILE *fp;

	fp = fopen("../sdm/memory_opencl_program.cl", "r");
	assert(fp);
	fseek(fp, 0L, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	kernelSource = (char*) malloc(sizeof(char)*(len+1));
	len = fread((void*)kernelSource, sizeof(char), len, fp);
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
	free(kernelSource);

	err = clBuildProgram(program, 0, NULL, "-I../sdm", NULL, NULL);
	if (err != CL_SUCCESS) {
		fprintf(stderr, "Error: Failed to build program executable.\n");
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		fprintf(stderr, "%s\n", buffer);
		assert(err == CL_SUCCESS);
	}

	kernel = clCreateKernel(program, "count", &err);
	assert(kernel && err == CL_SUCCESS);

	in_memory = clCreateBuffer(context, CL_MEM_USE_HOST_PTR, sizeof(hardlocation*)*sdm_sample, sdm_memory, &err);
	assert(in_memory);
	in_address = clCreateBuffer(context, CL_MEM_USE_HOST_PTR, sizeof(bitstring)*bs_len, address, &err);
	assert(in_address);
	in_counter = clCreateBuffer(context, CL_MEM_USE_HOST_PTR, sizeof(counter), &counter, &err);
	assert(in_counter);

	err = 0;
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &in_memory);
	assert(err == CL_SUCCESS);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &in_address);
	assert(err == CL_SUCCESS);
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &in_counter);
	assert(err == CL_SUCCESS);
	err = clSetKernelArg(kernel, 3, sizeof(unsigned int), &radius);
	assert(err == CL_SUCCESS);

	global = 100000;
	local = 2;
	counter = 0;
	radius = 451;
	err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
	assert(err == CL_SUCCESS);

	err = clFinish(commands);
	assert(err == CL_SUCCESS);

	printf("@@ counter = %d\n", counter);

	clReleaseMemObject(in_memory);
	clReleaseMemObject(in_address);
	clReleaseMemObject(in_counter);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);

	return counter;
}

