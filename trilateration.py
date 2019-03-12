import numpy as np
import matplotlib.pyplot as plt

#initialising centres of 3 circles(i.e ) position of transmitters
a1=
b1=

a2=
b2=

a3=
b3=


d1=#received distance1
d2=#received distance2
d3=#received distance3

#solving 2 common chord equations to get coordinates

A=np.array([-2*a1+2*a2,-2*b1+2*b2],[-2*a2+2*a3,-2*b2+2*b3]])
B=np.array([(d1*d1)-(d2*d2)-(b1*b1)+(b2*b2)-(a1*a1)+(a2*a2)],[(d2*d2)-(d3*d3)-(b2*b2)+(b3*b3)-(a2*a2)+(a3*a3)]])
coordinates=np.linalg.solve(A,B)
x=coordinates[0]
y=coordinates[1]
plt.plot(x,y,'bo')
