#!/usr/bin/env python

import unittest
import subprocess, os
from numpy import array
import sdm
from sdm import Bitstring, Hardlocation

class MemoryTestCase(unittest.TestCase):
    
    def _memused(self):
        p = subprocess.Popen("ps -p %d -o rss | grep '^ *[0-9]\+ *$'" % os.getpid(), shell=True, stdout=subprocess.PIPE)
        return int(p.communicate()[0])

    def test_initialize_free(self, qty=5):
        m0 = self._memused()
        for i in range(qty):
            sdm.initialize()
            sdm.free()
        m1 = self._memused()
        self.assertTrue(m1 < 40000)

    def test_mean_distance(self, qty=10):
        sdm.initialize()
        for i in range(qty):
            a = Bitstring()
            arr = array(sdm.distance(a))
            self.assertTrue(abs(arr.mean()-500) <= 1.5)

    def test_writeread(self, qty=10):
        sdm.initialize()
        for i in range(qty):
            a = Bitstring()
            sdm.write(a, a)
            b = sdm.read(a)
            self.assertEqual(a.distance_to(b), 0)

    def test_writereadnear(self, distance=50, qty=10):
        sdm.initialize()
        for i in range(qty):
            a = Bitstring()
            sdm.write(a, a)
            b = a.copy()
            b.bitrandomswap(distance)
            c = sdm.read(b)
            self.assertEqual(a.distance_to(c), 0)



if __name__ == '__main__':
    unittest.main()

