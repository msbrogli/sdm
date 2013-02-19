'''
Demonstration of quiver and quiverkey functions. This is using the
new version coming from the code in quiver.py.

Known problem: the plot autoscaling does not take into account
the arrows, so those on the boundaries are often out of the picture.
This is *not* an easy problem to solve in a perfectly general way.
The workaround is to manually expand the axes.

'''
import matplotlib
matplotlib.use('TkAgg')
#import sdm
from pylab import *
from numpy import ma
from numpy import random

X,Y = meshgrid( arange(0,2*pi,.2),arange(0,2*pi,.2) )
U = cos(X)
V = sin(Y)

#1
figure()

C = random.uniform(0.0,100.0, (len(X),len(X)))
Q = quiver( U, V, C)
qk = quiverkey(Q, 0.5, 0.92, 2, r'$\beta$', labelpos='W',
               fontproperties={'weight': 'bold'})
l,r,b,t = axis()
dx, dy = r-l, t-b
axis([l-0.05*dx, r+0.05*dx, b-0.05*dy, t+0.05*dy])

title('Address-decoding neurons', { 'fontname':'Tahoma', 'fontsize':20 })

show()