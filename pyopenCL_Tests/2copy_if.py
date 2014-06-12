import pyopencl as cl
import numpy as np

context = cl.create_some_context()
queue = cl.CommandQueue(context)
mf = cl.mem_flags


from pyopencl.scan import GenericScanKernel
scan_kernel = GenericScanKernel(
        context, np.int32,
        arguments="__global int *ary",
        input_expr="ary[i]",
        scan_expr="a+b", neutral="0",
        output_statement="ary[i+1] = item;")

a = cl.array.arange(queue, 10000, dtype=np.int32)
scan_kernel(a, queue=queue)

print a 