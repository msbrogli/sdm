#  cd /Users/AL/Dropbox/0. AL Current Work/3. To Submit/Dr K/AL/python/


import sdm
import sdm_utils
import time

def mem_write_x_at_x(count=1):
    for i in range (count):
        b=sdm.Bitstring()
        sdm.thread_write(b,b)

def mem_write_x_at_random(count=1):
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


def linhares_critical1():
    #cd /Users/AL/Dropbox/0. AL Current Work/3. To Submit/Dr K/AL/python/
    import sdm
    import sdm_utils
    import time
    start=time.clock()
    #sdm.initialize()
    sdm.initialize_from_file("/Users/AL/Desktop/mem45000_n1000_10000x_at_x.sdm")
    mem_write_x_at_x(5000)
    v = sdm.Bitstring()
    sdm.thread_write(v,v)
    print ("computing distances graph")
    print (time.clock()-start, "seconds") 
    a = sdm_utils.critical_distance2(0, 1000, 1, v)
    print (time.clock()-start) 
    print "saving file"
    sdm.save_to_file("/Users/AL/Desktop/mem50000_n1000_10000x_at_x.sdm")
    import pylab
    pylab.plot(a)
    pylab.show()


def scan_for_distances(numwrites=1):
    import time, cPickle;

    #Configure execution
    numwrites_string = '_x_at_x_'+str(numwrites)+'_writes'

    sdm.initialize()
    v = sdm.Bitstring()
    for i in range(numwrites):
        sdm.thread_write(v,v)
    for i in range (1000,2000,1000):
        print 'Registering '+str(i)+' new items in SDM'
        #add 1000 itens to memory
        mem_write_x_at_x(1000)
        print 'Computing the distances to these items'
        a = sdm_utils.critical_distance2(0, sdm.get_dimension(), 1, v, iterated_reading=6, read=sdm.thread_read_cubed,debug=0)
        #save new distance values in a
        picklename = 'N=256_iter_read=2_'+str(i)+'saved items'+numwrites_string+'_DrK_cubed.cPickle'
        cPickle.dump(a, open (picklename, 'wb'))
        print 'saved '+picklename
    sdm.free()

  
def Compute():
    sdm.set_thread_count(16)
    sdm.set_dimension(256)
    #YOU MUST COMPUTE THE PROPER RADIUS BEFORE CHANGING N, IDIOT!
    #if N=100, radius=451
    #if N=256, radius=103
    
    start = time.clock()
    sdm.set_radius(103)
    #test 1 & 10
    for numwrites in range(1):
        print (numwrites)
        scan_for_distances(numwrites)
    print (time.clock()-start) 

''' 
Experiment manipulations

Chada_Read
Drk_Read

x_at_x
NO-->x_at_random

1_write
10_writes

256
1000

Cubed

randomness at 0, 25%, 50%, 75%, 100% (x at random)


TIME WITH COREi5, popcount = 445.502513
Time with Marcelo = 376.663404

optimizing Marcelo's makefile... terminava no -O3!!! piorou: 400.376244
retornando para o makefile original, 377.13109!!
'''










