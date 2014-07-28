import pyopencl as cl
import numpy as np
from pyopencl import array
from pyopencl.algorithm import copy_if


context = cl.create_some_context()
queue = cl.CommandQueue(context)
mf = cl.mem_flags

rand = cl_array.to_device (ctx, queue, numpy.random.randn(4,4).astype(np.int32))

#rand = np.random.random_integers(0,2**10,size=(2**10)*8).astype(np.int32) 

print rand


a = array.to_device(queue, rand, allocator=None, async=False)


scan_kernel = copy_if(a, predicate = "(a[i]<104) ? 1:0")

#a = cl.array.arange(queue, 10000, dtype=np.int32)


out, count, event = scan_kernel(a, queue=queue)

print a 