#!/usr/bin/env python

import unittest
import subprocess, os
import sdm
from sdm import Bitstring, Hardlocation

class HardlocationTestCase(unittest.TestCase):
    
    def _memused(self):
        p = subprocess.Popen("ps -p %d -o rss | grep '^ *[0-9]\+ *$'" % os.getpid(), shell=True, stdout=subprocess.PIPE)
        return int(p.communicate()[0])

    def test_alloc_dealloc(self, qty=1000000):
        m0 = self._memused()
        for i in range(qty):
            a = Hardlocation()
            del a
        m1 = self._memused()
        self.assertTrue(m1 < 80000)

    def test_write_adder(self, qty=1000):
        for i in range(qty):
            a = Hardlocation()
            b = Bitstring()
            a.write(b)
            self.assertEqual([ b.bitsign(i) for i in xrange(sdm.get_dimension()) ], a.adder)
            a.write(b)
            self.assertEqual([ 2*b.bitsign(i) for i in xrange(sdm.get_dimension()) ], a.adder)

    def test_readwrite_single(self, qty=1000):
        for i in range(qty):
            a = Hardlocation()
            b = Bitstring()
            a.write(b)
            c = a.read()
            self.assertEqual(b.distance_to(c), 0)

    def test_readwrite_overflow(self, qty=1000):
        a = Hardlocation()
        b = Bitstring()
        for i in range(qty):
            a.write(b)
        c = a.read()
        self.assertEqual(b.distance_to(c), 0)

    def test_readwrite_swap(self, qty=1000):
        for i in range(qty):
            a = Hardlocation()
            b = Bitstring()
            a.write(b)
            c = a.read()
            self.assertEqual(b.distance_to(c), 0)

            b.bitswap(0)
            a.write(b)
            a.write(b)
            c = a.read()
            b.bitswap(0)

            self.assertEqual(b.distance_to(c), 1)
            c.bitswap(0)
            self.assertEqual(b.distance_to(c), 0)


if __name__ == '__main__':
    sdm.set_dimension(256)
    sdm._libsdm.bs_initialize()
    unittest.main()

