import matplotlib.pyplot as plt
import matplotlib.animation as anim
import math
import numpy as np
import bluetooth
import sys
import time
'''
#object pos
_x = 0
_y = 0

#positions of transmitters
## a -> x
## b -> y
a1 = 0
a2 = 1
a3 = 0.5

b1 = 0
b2 = 0
b3 = 0.866

fig = plt.figure()
ax1 = fig.add_subplot(111)

def refreshGraphData(i):
    ax1.clear()
    ax1.plot(_x, _y)

ani = anim.FuncAnimation(fig, refreshGraphData, interval=1000)
plt.show()
'''

nearby_devices = bluetooth.discover_devices()

num = 0
print ("Select your device by entering its coresponding number...")
for i in nearby_devices:
    num += 1
    print (num , ": " , bluetooth.lookup_name( i ))
selection = int(input("> ")) - 1

print ("You have selected", bluetooth.lookup_name(nearby_devices[selection]))


bd_addr = nearby_devices[selection]
port = 1
sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
sock.connect((bd_addr, port))


#initialize to arbitrary point
d1 = 0.577
d2 = 0.577
d3 = 0.577

#   data format
#   _id - dist

while 1:
#    data = sock.recv(1023)
    data = input("> :: ")
    #match data with preset
    start = data[0:data.index('-')]
    end = float(data[data.index('-') + 1:len(data)].strip(' "'))
    #set _d1, _d2, _d3
    if start == '0':
        d1 = end
    elif start == '1':
        d2 = end
    elif start == '2':
        d3 = end

    #solving 2 common chord equations to get coordinates
    A = np.array ( ( [ -2*a1 + 2*a2 , -2*b1 + 2*b2 ], [ -2*a2 + 2*a3, -2*b2 + 2*b3 ] ) )
    B = np.array ( ( [ d1*d1 - d2*d2 - b1*b1 + b2*b2 - a1*a1 + a2*a2 ], [ d2*d2 - d3*d3 - b2*b2 + b3*b3 - a2*a2 + a3*a3 ] ) )
    coordinates = np.linalg.solve (A, B)
    _x = coordinates[0]
    _y = coordinates[1]
    print(x, y)
    #TODO: Add exception handler to exit loop

sock.close()