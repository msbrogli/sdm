#!/usr/bin/env python

from pylab import *

y = [ int(x) for x in open('out1_value.txt') ]
x = range(len(y))

plot(x, y, linewidth=1.0)

xlabel('radius')
ylabel('neighbourhood')

grid(True)
show()

