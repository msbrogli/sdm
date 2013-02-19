#  cd /Users/AL/Dropbox/0. AL Current Work/3. To Submit/Dr K/AL/python/



import sdm
import sdm_utils
from numpy import *

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


def scan_for_distances():
    import time, cPickle;
    sdm.initialize()
    v = sdm.Bitstring()
    for i in range (0,10,1):	
        sdm.thread_write(v,v)
    import pylab
    for i in range (1000,51000,1000):
        print 'Computing distances for '+str(i)+' items registered'
        #add 1000 itens to memory
        mem_write_x_at_x(1000)
        a = sdm_utils.critical_distance2(0, 1000, 1, v, read=sdm.thread_read_chada)
        #get new distance values in a
        #save a
        cPickle.dump(a, open (str(i)+'10writes_Chada_Read.cPickle', 'wb'))
        print 'saved '+str(i)+'.cPickle'
    #print 'now lets see..'
    #for i in range (1000,11000,1000):
    #    print (cPickle.load(open(str(i)+'.cPickle','rb')))
  


#from pylab import *
def TestFig1():
    import os, cPickle
    #os.chdir ("results/6_iter_readng/1000D/DrK_Read/x_at_x/")
    import pylab
    for i in range (1000,51000,1000):
        a = (cPickle.load(open(str(i)+'_10writes.cPickle','rb')))
        pylab.plot(a)
    pylab.show()




from matplotlib.pylab import *
def Plot_Heatmap (data=[]):
    # Make plot with vertical (default) colorbar
    maxd = int(data.max())
    mind = int(data.min())
    avgd = int ((maxd+mind) / 2);
    print 'minimum value=',mind
    fig = plt.figure()
    ax = fig.add_subplot(111)


    #use aspect=20 when N=1000
    #use aspect=5 when N=256
    
    cax = ax.imshow(data, cmap=cm.YlGnBu, aspect=5.0, interpolation=None, norm=None, origin='lower')
    ax.set_title('Critical Distance Behavior', fontsize=58)
    ax.grid(True, label='Distance')
    ax.set_xlabel('original distance', fontsize=100)
    ax.set_ylabel("# items previously stored (000's)")

    # Add colorbar, make sure to specify tick locations to match desired ticklabels
    cbar = fig.colorbar(cax, ticks=[mind, avgd, maxd]) #had ZERO here before
    cbar.ax.set_yticklabels([str(mind), str(avgd), str(maxd)])
    cbar.ax.set_ylabel('distance obtained after 20 iteractive-readings', fontsize=24)
        
    #########CONTOUR DELINEATES THE CRITICAL DISTANCE

    # We are using automatic selection of contour levels;
    # this is usually not such a good idea, because they don't
    # occur on nice boundaries, but we do it here for purposes
    # of illustration.
    CS = contourf(data, 100, levels = [mind,avgd,maxd], alpha=0.1, cmap=cm.YlGnBu, origin='lower')
    
    # Note that in the following, we explicitly pass in a subset of
    # the contour levels used for the filled contours.  Alternatively,
    # We could pass in additional levels to provide extra resolution,
    # or leave out the levels kwarg to use all of the original levels.

    CS2 = contour(CS,  levels=[88], colors = 'gray', origin='lower', hold='on', linestyles='dashdot')
    
    title('Critical Distance Behavior', fontsize=40)
    xlabel('original distance', fontsize=24)
    ylabel("# items previously stored (000's)", fontsize=24)
    # Add the contour line levels to the colorbar
    #cbar.add_lines(CS2)
    show()


from matplotlib.pylab import *
import os, cPickle
def GetDataForPlots(folder='',filenameext='MUST_BE_PROVIDED'):
    p=q=r=s=[]
    if len(folder)>0: os.chdir (folder)
    for i in range(1,51):
        S = 'N=256_iter_read=2_'+str(i*1000)+filenameext+'.cPickle'
        p.append( (cPickle.load(open(S,'rb') ) ) )
    q=concatenate(p,axis=0)
    r = q[:,1]
    print len(r)
    print '& shape (r)=',shape(r)
    r.shape=(50,256)  #if N=256 
    #r.shape=(50,1000)
    print 'r=',r
    return r
    

def now():
    #data=GetDataForPlots("results/6_iter_readng/1000D/DrK_Read/x_at_x/1_write", '')
    #data=GetDataForPlots("results/6_iter_readng/1000D/DrK_Read/x_at_x/10_writes", '_10writes')
    data=GetDataForPlots('','saved items_x_at_x_0_writes_DrK_cubed')
    Plot_Heatmap (data)

