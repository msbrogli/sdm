#!/usr/bin/env python

import unittest
import sdm
from sdm import Bitstring

class BitstringTestCase(unittest.TestCase):

    def setUp(self):
        pass

    def test_distance_zero(self, qty=1000):
        for i in range(qty):
            a = Bitstring()
            self.assertEqual(Bitstring.distance(a, a), 0)

    def test_distance_random(self, qty=1000):
        for i in range(qty):
            a = Bitstring()
            b = Bitstring()
            dist = sum([ 1 for x, y in zip(str(a), str(b)) if x != y ])
            self.assertEqual(Bitstring.distance(a, b), dist)

    def test_bitset(self):
        dim = sdm.get_dimension()
        a = Bitstring(zero=True)
        for i in range(dim):
            a.bitset(i)

            s = str(a)
            s1 = sum([ 1 for x in s[:dim-i-1] if x == '0' ])
            s2 = sum([ 1 for x in s[-i-1:] if x == '1' ])
            self.assertEqual(s1+s2, dim)

    def test_bitclear(self):
        dim = sdm.get_dimension()
        a = Bitstring(zero=True)
        for i in range(dim):
            a.bitset(i)
        self.assertEqual(sum([ 1 for x in str(a) if x == '1' ]), dim)

        for i in range(dim):
            a.bitclear(i)

            s = str(a)
            s1 = sum([ 1 for x in s[:dim-i-1] if x == '1' ])
            s2 = sum([ 1 for x in s[-i-1:] if x == '0' ])
            self.assertEqual(s1+s2, dim)


if __name__ == '__main__':
    unittest.main()

