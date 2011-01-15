#!/usr/bin/env python

import sdm
from time import time
from sdm import Bitstring, Hardlocation

def write_random(qty, use_threads=True):
    w = sdm.write if not use_threads else sdm.thread_write
    t0 = time()
    for i in range(qty):
        a = Bitstring()
        print '#%d'%i, w(a, a)
    t1 = time()
    print 'Time:', t1-t0, 'seconds'

def sample():
    print 'Initializing SDM'
    sdm.initialize()
    dimension = sdm.get_dimension()
    addr = Bitstring()
    dist = sdm.distance(addr)
    sdm.free()
    return dist

def sample_radius(rounds=1, verbose=0):
    if verbose > 0:
        print 'Initializing SDM'
    sdm.initialize()
    dimension = sdm.get_dimension()
    v = []
    for i in xrange(rounds):
        if verbose > 0:
            print 'Round #%d' % (i+1)
        addr = Bitstring()
        
        dist = sdm.distance(addr)

        if verbose > 0:
            print '  Processing results'
        w = [0]*(dimension+1)
        for d in dist:
            w[d] += 1
        u = [0]*(dimension+1)
        for d, n in enumerate(w):
            for j in range(d, dimension+1):
                u[j] += n
        v.append(u)
    sdm.free()
    return v

def chart_sample_radius(rounds=1, verbose=0):
    from pylab import *
    data = sample_radius(rounds=rounds, verbose=verbose)
    #for i in range(sdm.get_dimension()+1):
    #    data[-1][i] += 100000

    dist = range(sdm.get_dimension()+1)
    args = []
    kwargs = { 'linewidth': 1.0 }
    for v in data:
        args.extend([ dist, v, 'k-' ])
    plot(*args, **kwargs)

    xlabel('distance')
    ylabel('qty of hardlocations')
    title('Qty of hardlocations around random address')
    grid(True)
    show()

    arr = array(zip(*data))
    stat = [ (i, m, v) for i, (m, v) in enumerate(zip(arr.mean(axis=1), arr.std(axis=1))) ]

    return data, stat

