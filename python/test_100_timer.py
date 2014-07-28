#!/usr/bin/env python

import sdm
import time

print 'Initializing..',
sdm.initialize()
print 'done!'

#print 'Running count..'
#start = time.time()
#for i in range(300):
#    a = sdm.Bitstring()
#    print i, sdm.thread_radius_count(a)
#print 'Time:', time.time()-start

print 'Running write..'
start = time.time()
for i in range(300):
    a = sdm.Bitstring()
    print i, sdm.thread_write(a, a)
print 'Time:', time.time()-start

