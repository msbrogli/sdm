import pyopencl as cl
import pyopencl.array 
import numpy
import numpy.linalg as la
import time



HARD_LOCATIONS = 2**20
EXPECTED_NUM_HARD_LOCATIONS = 1250
HASH_TABLE_SIZE =  12043 # 25033 #12043  #Must be a prime number in the OpenCL code

# HASH_TABLE_SIZE must be prime.  The higher it is, the more bandwidth, but way less collisions.


print "HASH_TABLE_SIZE=", HASH_TABLE_SIZE  #WHAT IS THE OPTIMUM HASH_TABLE_SIZE??

ACCESS_RADIUS_THRESHOLD = 104 #COMPUTE EXPECTED NUMBER OF num_ACTIVE_locations_found HARD LOCATIONS



numpy.random.seed(seed=12345678)  

ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)
mem_flags = cl.mem_flags


def Get_Bin_Active_Indexes():
	bin_active_index = numpy.zeros(HASH_TABLE_SIZE).astype(numpy.uint32) 
	return bin_active_index

def Get_Bin_Active_Indexes_GPU_Buffer(ctx):
	bin_active_index = Get_Bin_Active_Indexes()
	bin_active_index_gpu = cl.Buffer(ctx, mem_flags.READ_WRITE | mem_flags.COPY_HOST_PTR, hostbuf=bin_active_index)
	return bin_active_index_gpu

def Get_Hamming_Distances():
	hamming_distances = numpy.zeros(HARD_LOCATIONS).astype(numpy.uint32) 
	hamming_distances[0] = ACCESS_RADIUS_THRESHOLD
	return hamming_distances

def Get_Distances_GPU_Buffer(ctx):
	Distances = Get_Hamming_Distances() 
	hamming_distances_gpu = cl.Buffer(ctx, mem_flags.READ_WRITE | mem_flags.COPY_HOST_PTR, hostbuf=Distances) 
	return hamming_distances_gpu

def Get_Bitstring_GPU_Buffer(ctx):
	bitstring = numpy.random.random_integers(0,2**32,size=8).astype(numpy.uint32)
	bitstring_gpu = cl.Buffer(ctx, mem_flags.READ_ONLY | mem_flags.COPY_HOST_PTR, hostbuf=bitstring)
	return bitstring_gpu

def Create_Memory_Addresses():
	memory_addresses = numpy.random.random_integers(0,2**32,size=(2**20)*8).astype(numpy.uint32)
	return memory_addresses

def Get_Memory_Addresses_Buffer(ctx):
	memory_addresses = Create_Memory_Addresses() 	
	memory_addresses_gpu = cl.Buffer(ctx, mem_flags.READ_ONLY | mem_flags.COPY_HOST_PTR, hostbuf=memory_addresses)
	return memory_addresses_gpu

def Get_Text_code(filename):
	with open (filename, "r") as myfile:
	    data = myfile.read()
	    return data









bin_active_index_gpu = Get_Bin_Active_Indexes_GPU_Buffer(ctx)
memory_addresses_gpu = Get_Memory_Addresses_Buffer(ctx)
distances_gpu = Get_Distances_GPU_Buffer(ctx)

bitstring_gpu = Get_Bitstring_GPU_Buffer(ctx)



OpenCL_code = Get_Text_code ('GPU_Code_OpenCLv1_2.cl')

import os
os.environ['PYOPENCL_COMPILER_OUTPUT'] = '1'

start = time.time()

prg = cl.Program(ctx, OpenCL_code).build()

#from pyopencl.scan import GenericScanKernel
bin_active_index = Get_Bin_Active_Indexes()
hamming_distances = Get_Hamming_Distances()


print "\n\n"

Results = numpy.zeros(HASH_TABLE_SIZE).astype(numpy.uint32) 

#bin_active_index_gpu = Get_Bin_Active_Indexes_GPU_Buffer(ctx)

start = time.time()
num_times = 2000
for x in range(num_times):
	
	bitstring_gpu = Get_Bitstring_GPU_Buffer(ctx)
	
	err = prg.clear_bin_active_indexes_gpu(queue, (HASH_TABLE_SIZE,), None, bin_active_index_gpu).wait()

	err = prg.get_active_hard_locations(queue, (HARD_LOCATIONS,), None, memory_addresses_gpu, bitstring_gpu, distances_gpu, bin_active_index_gpu).wait()  
	if err: print 'Error --> ',err

	
	bin_active_index = Get_Bin_Active_Indexes() #THIS IS SLOWING EVERYTHING!


	err = cl.enqueue_read_buffer(queue, bin_active_index_gpu, bin_active_index).wait()
	if err: print 'Error in retrieving bin_active_index? --> ',err

	# Removing zeros from bin_active_index, 2 options:
	# option 1: use numpy masked arrays
	# bin_active_index = numpy.ma.masked_equal(bin_active_index,0).compressed()
	# option 2: a[a != 0]
	bin_active_index = bin_active_index[bin_active_index!=0]
	

	#print bin_active_index
	num_active_locations_found = numpy.size(bin_active_index)
	#print  "num_active_locations_found=", num_active_locations_found
	Results[x] = num_active_locations_found
	
	time_elapsed = (time.time()-start)
	#if (x%1==0): print x, time_elapsed, "\n\n"

time_elapsed = (time.time()-start)

mean  = Results[Results !=0].mean()

print Results[Results !=0].min(), " the minimum of HLs found should be 980"
print mean, "the mean of HLs found should be 1094.7665"
print Results[Results !=0].max(), "the max of HLs found should be 1220"


print numpy.size(bin_active_index)

print bin_active_index

print hamming_distances[bin_active_index]

print '\nTime to compute some Hamming distances', num_times,'times:', time_elapsed

# why bin?  sum = numpy.sum(bin_active_index)
sum = numpy.sum(Results)

print '\n Sum of num_active_locations_found locations = ', sum, "error =", sum-2238071, "\n"
print 'expected HLs missed per scan is', (2238071-sum)/num_times

# 2189533 single hashing in OpenCL code
# 2236635 double-hashing in OpenCL code
# 2238019 triple-hashing in OpenCL code
# 2238069 quadruple-hashing in OpenCL code
# 2238071 quintuple-hashing in OpenCL code


# RETRIEVE num_ACTIVE_locations_found HARDLOCATIONS!
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