import matplotlib.pyplot as plt
import matplotlib.animation as anim
import math
import numpy as np
import bluetooth
import sys
import time

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

while 1:
    #   data format
    #   _id - dist
    data = sock.recv(1023)
    print (data)

sock.close()