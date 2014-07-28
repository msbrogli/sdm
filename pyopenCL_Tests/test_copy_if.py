#from https://github.com/pyopencl/pyopencl/blob/master/test/test_algorithm.py

def test_copy_if(ctx_factory):
    from pytest import importorskip
    importorskip("mako")

    context = ctx_factory()
    queue = cl.CommandQueue(context)

    from pyopencl.clrandom import rand as clrand
    for n in scan_test_counts:
        a_dev = clrand(queue, (n,), dtype=np.int32, a=0, b=1000)
        a = a_dev.get()

        from pyopencl.algorithm import copy_if

        crit = a_dev.dtype.type(300)
        selected = a[a > crit]
        selected_dev, count_dev, evt = copy_if(
                a_dev, "ary[i] > myval", [("myval", crit)])

        assert (selected_dev.get()[:count_dev.get()] == selected).all()
        from gc import collect
        collect()



