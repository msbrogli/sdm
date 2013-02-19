#!/usr/bin/env python

import sdm
import sys, random
from time import time
from sdm import Bitstring, Hardlocation

def test_uniform_distribution(qty=10000):
    n = sdm.get_dimension()
    v = [0]*n
    for i in xrange(qty):
        a = Bitstring()
        for j in range(n):
            v[j] += a.bitsign(j)
    import pylab
    pylab.hist(v, bins=15)
    return v

def table_7_1():
    n = sdm.get_dimension()
    a = Bitstring()
    v = []
    for i in range(550):
        b = a.copy()
        b.bitrandomswap(i)
        v.append([ i, sdm.thread_radius_count_intersect(a, b) ])
        print i
        sys.stdout.flush()
    return v


def critical_distance(a, b, n, v, read=sdm.thread_read, debug=0):
    ret = []
    for i in xrange(a, b):
        ret2 = []
        for j in xrange(n):
            u = v.copy()
            u.bitrandomswap(i)
            w = read(u)
            d = v.distance_to(w)
            if debug>1:
                print '  ', i, j, d
            ret2.append(d)
        d = 1.0*sum(ret2)/len(ret2)
        if debug>0:
            print '#%d'%i, d
            sys.stdout.flush()
        ret.append([ i, d ])
    return ret


def critical_distance2(a, b, n, v, iterated_reading=6, read=sdm.thread_read, debug=0):
    ret = []
    for i in xrange(a, b):
        ret2 = []
        for j in xrange(n):
            u = v.copy()
            u.bitrandomswap(i)
            w = read(u)
            for k in range (iterated_reading-1):
            	w = read (w)
            d = v.distance_to(w)
            if debug>1:
                print '  ', i, j, d
            ret2.append(d)
        d = 1.0*sum(ret2)/len(ret2)
        if debug>0:
            print '#%d'%i, d
            sys.stdout.flush()
        ret.append([ i, d ])
    return ret

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
    import pylab
    data = sample_radius(rounds=rounds, verbose=verbose)
    #for i in range(sdm.get_dimension()+1):
    #    data[-1][i] += 100000

    dist = range(sdm.get_dimension()+1)
    args = []
    kwargs = { 'linewidth': 1.0 }
    for v in data:
        args.extend([ dist, v, 'k-' ])
    pylab.plot(*args, **kwargs)

    pylab.xlabel('distance')
    pylab.ylabel('qty of hardlocations')
    pylab.title('Qty of hardlocations around random address')
    pylab.grid(True)
    pylab.show()

    arr = array(zip(*data))
    stat = [ (i, m, v) for i, (m, v) in enumerate(zip(arr.mean(axis=1), arr.std(axis=1))) ]

    return data, stat

