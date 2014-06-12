import pyopencl as cl
import pyopencl.array 
import numpy
import numpy.linalg as la
import time


DIMENSIONS = 256
HARD_LOCATIONS = 2**20
INT_SIZE = 256 
NUM_INTS = DIMENSIONS/INT_SIZE # why?  Because N=256 / 32 bits (usando uint32)
SIZE = HARD_LOCATIONS * NUM_INTS
ACCESS_RADIUS_THRESHOLD = 100

numpy.random.seed(seed=12345678)  

semaphor = numpy.zeros(2).astype(numpy.uint32) 

ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)
mem_flags = cl.mem_flags


def Get_Hamming_Distances():
	hamming_distances = numpy.zeros(2**20).astype(numpy.int32) 
	return hamming_distances

def Get_Distances_Buffer(ctx):
	Distances = Get_Hamming_Distances()
	hamming_distances_buffer = cl.Buffer(ctx, mem_flags.READ_WRITE | mem_flags.COPY_HOST_PTR, hostbuf=hamming_distances)
	return hamming_distances_buffer

def Get_Bitstring_Buffer(ctx):
	bitstring = numpy.random.random_integers(0,2**32,size=8).astype(numpy.uint32)
	bitstring_buffer = cl.Buffer(ctx, mem_flags.READ_ONLY | mem_flags.COPY_HOST_PTR, hostbuf=bitstring)
	return bitstring_buffer

def Create_Memory_Addresses():
	memory_addresses = numpy.random.random_integers(0,2**32,size=(2**20)*8).astype(numpy.uint32)
	return memory_addresses

def Get_Memory_Addresses_Buffer(ctx):
	memory_addresses = Create_Memory_Addresses() 	
	memory_addresses_buffer = cl.Buffer(ctx, mem_flags.READ_ONLY | mem_flags.COPY_HOST_PTR, hostbuf=Create_Memory_Addresses())
	return memory_addresses_buffer

def Get_Text_code(filename):
	with open (filename, "r") as myfile:
	    data = myfile.read()
	    return data



memory_addresses = Create_Memory_Addresses()
memory_addresses_buffer = Get_Memory_Addresses_Buffer(ctx)

bitstring_buffer = Get_Bitstring_Buffer(ctx)

hamming_distances = Get_Hamming_Distances()
distances_buffer = Get_Distances_Buffer(ctx)

OpenCL_code = Get_Text_code ('GPU_Code_OpenCLv1_2.cl')

import os
os.environ['PYOPENCL_COMPILER_OUTPUT'] = '1'

start = time.time()

prg = cl.Program(ctx, OpenCL_code).build()

from pyopencl.scan import GenericScanKernel

start = time.time()
num_times = 200
for x in range(num_times):
	prg.compute_hammings_hard_locations_256bits(queue, (HARD_LOCATIONS,), None, memory_addresses_buffer, bitstring_buffer, distances_buffer).wait()  
	time_elapsed = (time.time()-start)
	if (x%100==0): print x, time_elapsed
  


err = cl.enqueue_read_buffer(queue, distances_buffer, hamming_distances).wait()
if err: print 'Error in computing hamming_distances? --> ',err

time_elapsed = (time.time()-start)
print 'Time to compute some Hamming distances', num_times,'times:', time_elapsed

#numpy.set_printoptions(threshold='nan')
#print hamming_distances
print '\n Sum of distances at:', numpy.sum(hamming_distances)


# RETRIEVE ACTIVE HARDLOCATIONS!
#==================================
'''
from pyopencl.scan import GenericScanKernel

knl = GenericScanKernel(
        ctx, numpy.int32,
        arguments="__global int *ary, __global int *out",
        input_expr="(ary[i] < 104) ? 1 : 0",
        scan_expr="a+b", neutral="0",
        output_statement="""if (prev_item != item) out[item-1] = ary[i];""")

#ary = hamming_distances.copy()
out = hamming_distances.copy()
knl(hamming_distances, out).wait()

a_host = ary.get()
out_host = a_host[a_host < 104]

assert (out_host == out.get()[:len(out_host)]).all()
'''