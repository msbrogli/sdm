import pyopencl as cl
import pyopencl.algorithm
from pyopencl.scan import GenericScanKernel
import numpy as np


ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)
mf = cl.mem_flags

knl = GenericScanKernel(
        ctx, np.int32,
        arguments="__global int *ary, __global int *out",
        input_expr="(ary[i] < 104) ? 1 : 0",
        scan_expr="a+b", neutral="0",
        output_statement="""if (prev_item != item) out[item-1] = ary[i];""")

ary = cl.array.arange(queue, 10000, dtype=np.int32)
print ary
out = ary.copy()
knl(ary, out)

a_host = ary.get()
out_host = a_host[a_host < 104]

 
print out

#code = open("knl.cl", "w").write(knl)

#print code

assert (out_host == out.get()[:len(out_host)]).all()