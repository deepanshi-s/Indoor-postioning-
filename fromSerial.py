import matplotlib.pyplot as plt
import numpy as np

import serial

a1=#
b1=#

a2=#
b2=#

a3=#
b3=#

#portname to be given
Data=serial.Serial('com9',9600)
while True:
    plt.clf()
    while (Data.inWaiting()==0):
        pass
    coord=str(Data.readline())
    coord=coord.replace('b\'','')
    coord=coord.replace('\\r\\n\'','')
    coordarr=coord.split(',')
   
    d1=int(coordarr[0])
    d2=int(coordarr[1])
    d3=int(coordarr[2])
    #print(d1)
    A = np.array ( ( [ -2*a1 + 2*a2 , -2*b1 + 2*b2 ], [ -2*a2 + 2*a3, -2*b2 + 2*b3 ] ) )
    B = np.array ( ( [ d1*d1 - d2*d2 - b1*b1 + b2*b2 - a1*a1 + a2*a2 ], [ d2*d2 - d3*d3 - b2*b2 + b3*b3 - a2*a2 + a3*a3 ] ) )
    coordinates = np.linalg.solve (A, B)
    _x = coordinates[0]
    _y = coordinates[1]
    
    plt.pause(1)
    plt.ylim(0,10)#will be set later
    plt.xlim(0,10)
    plt.plot(_x,_y,"bo")
    
    #plt.show()
    
   
