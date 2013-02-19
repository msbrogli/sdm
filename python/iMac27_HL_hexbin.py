#  cd /Users/AL/Dropbox/0. AL Current Work/3. To Submit/Dr K/AL/python/

#Here we draw a heatmap (actually a hexbin object), showing how the read
#distance converges when inside the critical distance, and diverges when 
#outside of it.

#We start with Kanerva's example: N=1000, 1.000.000HLs, and 10.000 items
#in memory. Then we make the following experiments:

#Experiment #1. Save r at r.  Read at random location; save that location, save the read address, read it again, and again for a third time.  Do this for a large number of iterations, until the hexbin looks good.

#Experiment #2. Save r at r (8 times) and s at s (4 times). Rinse Repeat.

#Experiment #3. Save r at r (8 times), s at s (4 times), and rAVGs (1 time). Rinse repeat. There will probably be now a "tunnel" linking r and s. Then again, maybe not.  The experiment will tell

#Problem: How to get 2 vectors? The bigmac has finished processing, so I'll come back to this after I see the fucking new graph. 

import sdm
import sdm_utils
from pylab import *
from matplotlib import pyplot as PLT
from matplotlib import cm as CM
from matplotlib import mlab as ML
import numpy as NP

def mem_write_x_at_x(count=10):
    for i in range (count):
        b=sdm.Bitstring()
        sdm.thread_write(b,b)

def mem_write_x_at_random(count=10):
    for i in range (count):
        b=sdm.Bitstring()
        c=sdm.Bitstring()
        sdm.thread_write(b,c)


def linhares_fig7_1():
    import sdm
    import sdm_utils
    sdm.initialize()
    a = sdm_utils.table_7_1()
    import pylab
    pylab.plot(a)
    pylab.show()



def heatmap(a,b):
    gridsize=100
    PLT.subplot(111)
    # if "bins=None", then color of each hexagon corresponds directly to its count
    # "C" is optional--it maps values to x, y coordinates; if C is None (default) then 
    # the result is a pure 2D histogram 
    PLT.hexbin(a, b, C=None, gridsize=gridsize, cmap=CM.YlGnBu, bins=None)
    PLT.axis([a.min(), a.max(), b.min(), b.max()])
    cb = PLT.colorbar()
    cb.set_label('mean value')
    PLT.show() 


def h():
    import sdm
    import sdm_utils
    #import numpy as NP
    a = b = arange (0,1000)
    sdm.initialize_from_file("/Users/AL/Desktop/mem10000_n1000_10000x_at_x.sdm")
    v = sdm.Bitstring()
    sdm.thread_write(v,v)
    print ("computing distances")
    #for i in range (0,999,250):
        #a = sdm_utils.critical_distance2(0, 10, 1, v)
        #b = sdm_utils.critical_distance2(0, 10, 1, v)
    heatmap (a,b)
