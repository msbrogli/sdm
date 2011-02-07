#!/usr/bin/env python

import sdm
import pylab
import sys, random
from matplotlib import pyplot
from time import time
from math import sqrt
from sdm import Bitstring, Hardlocation

class Projection2D(object):
    def __init__(self, a, b, c, name_a=None, name_b=None, name_c=None, figure_opts=None):
        self.bitstrings = [ a, b, c ]
        self.elements = self._create_base(a, b, c)
        self.names = [ name_a or 'a', name_b or 'b', name_c or 'c' ]
        self.figure = pyplot.figure(**(figure_opts or {}))
        pyplot.plot([ e.real for e in self.elements ], [ e.imag for e in self.elements ], 'ko')
        pyplot.plot([ e.real for e in self.elements ] + [ self.elements[0].real ], [ e.imag for e in self.elements ] + [ self.elements[0].imag ], 'k--')
        textcoords = [ (-30, -30), (30, -30), (-30, 30) ]
        for name, e, tc in zip(self.names, self.elements, textcoords):
            pyplot.annotate(
                name,
                xy=(e.real, e.imag), xycoords='data', 
                xytext=tc, textcoords='offset points', 
                arrowprops=dict(arrowstyle='->', connectionstyle='arc3,rad=.2')
            )
        axis = pyplot.gca()
        axis.set_xlim(-70, self.elements[1].real+70)
        axis.set_ylim(-70, self.elements[2].imag+70)

    def _create_base(self, a, b, c):
        """Maps bitstring to a nice representation in R^2.
        Rules:
         - a is always origin;
         - b is always on x axis, that is, b.y=0;
         - c is always on first quadrant (x>=0, y>=0)"""
        d0 = a.distance_to(b)
        d1 = a.distance_to(c)
        d2 = b.distance_to(c)
        cx = 1.0*(d0*d0 + d1*d1 - d2*d2)/2/d0
        cy2 = d1*d1 - cx*cx
        if cy2 < 0:
            raise ValueError()
        cy = sqrt(cy2)
        return [ 0, d0, cx+cy*1j ]

    def add_bitstring(self, x, label=None, fmt='k,', radius=None):
        """According to a base, maps a bitstring to a point inside
        base triangle."""
        p = 0
        distances = [ e.distance_to(x) for e in self.bitstrings ]
        if radius:
            ignore = True
            for d in distances:
                if d <= radius:
                    ignore = False
            if ignore:
                return None
        #print distances
        total = sum(distances)
        factor = 100.0
        weights = [ (factor/(1.0+factor*d/total) - factor/(1+factor))**2 for d in distances ]
        totalweights = sum(weights)
        for te, w in zip(self.elements, weights):
            p += (w/totalweights)*te
        pyplot.figure(self.figure.number)
        pyplot.plot(p.real, p.imag, fmt)
        if label:
            pyplot.annotate(
                label,
                xy=(p.real, p.imag), xycoords='data', 
                xytext=(-30, 30), textcoords='offset points', 
                arrowprops=dict(arrowstyle='->', connectionstyle='arc3,rad=.2')
            )
        return p

    def add_all_hardlocations(self, radius=None):
        n = sdm.get_sample()
        for i in xrange(n):
            hl = sdm.get_memory(i)
            self.add_bitstring(hl.address, radius=radius)
            print '#', i

    def test_distance(self, x, a, b, step=1):
        for i in xrange(a, b+1, step):
            y = x.copy()
            y.bitrandomswap(i)
            self.add_bitstring(y, label='%d'%i)

    def add_iterative_read(self, address, steps=6):
        d = address
        last = self.add_bitstring(d)
        for i in xrange(steps):
            d = sdm.thread_read(d)
            print '#%d'%i, d.distance_to(address)
            pos = self.add_bitstring(d)
            dif = last-pos
            pyplot.arrow(last.real, last.imag, dif.real, dif.imag, label='%d'%i)
            last = pos
        return d


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

