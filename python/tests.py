#!/usr/bin/env python

import unittest
import sdm
from sdm import Bitstring

class BitstringTest(unittest.TestCase):

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


if __name__ == '__main__':
    unittest.main()

