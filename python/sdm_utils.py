#!/usr/bin/env python

import sdm
from sdm import Bitstring, Hardlocation

def sample_radius(qty=1, verbose=0):
    sdm.initialize()
    v = []
    for i in xrange(qty):
        if verbose > 0:
            print 'Round #%d' % i
        addr = Bitstring()
        u = []
        for j in xrange(sdm.get_dimension()+1):
            u.append(sdm.radius_count(addr, j))
            if verbose > 1:
                print '  radius=%d counter=%d' % (j, u[-1])
        v.append(u)
    sdm.free()
    return v

