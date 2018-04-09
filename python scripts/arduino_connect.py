import time
import serial
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import csv
import numpy as np


data = open("scan_data.txt",'w')
data.close()
data = open("scan_data.txt",'a',)
stop = 0
ser = serial.Serial('COM6', 115200)
ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)
for i in range(24000):


    x = ser.readline()

    print(repr(x))
    #xnum = float(x)
    if x == 'done\r\n':
        break

    else:
      #print(x)
      data.write(x)
     #print(type(x))
data.close()
ser.close

z = []
theta = []
rho = []
with open('scan_data.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        z.append(float(row[0]))
        theta.append(float(row[1]))
        rho.append(float(row[2]))

z = [zee*4*0.32 for zee in z]
theta = [thee*0.03141592653 for thee in theta]
rho = [53.1 - rhee for rhee in rho]
x2 = [50,50,-50,-50,50,50,-50,-50]
y2 = [50,-50,50,-50,50,-50,50,-50]
z2 = [0,0,0,0,100,100,100,100]
def pol2cart(rh, th):
     x1 = rh* np.cos(th)
     y1 = rh * np.sin(th)
     return(x1,y1)
(x1, y1) = pol2cart(rho,theta)
k=0
user = raw_input("enter file name ")
filename = ['C:\Users\mb235713\Documents\SCANNER POINT CLOUDS\ ',user,'.txt']
filename =''.join(filename)


trans = open(filename,'w+')
a = np.asarray([ x1, y1, z ])
a = a.T
np.savetxt(trans.name, a, delimiter=",")
fig = plt.figure(figsize=(8,8),dpi=100)
ax1 = fig.add_subplot(111, projection='3d')
ax1.scatter(x1,y1,z)
#plt.hold(True)
#ax2 = fig.add_subplot(111, projection='3d')
ax1.scatter(x2,y2,z2,c='w')
plt.show()



#     x = ser.readline()
#     if x == 'stop':
#         stop = 1
#
#     else:
#         data_save
#
# data.close()
# ser.close


