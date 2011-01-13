#!/usr/bin/env python

import sdm
from sdm import Bitstring, Hardlocation

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

    return data
