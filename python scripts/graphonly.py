import time


import serial
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import csv
import numpy as np
data = open("meseeks_new.txt",'r')
data.close()
z = []
theta = []
rho = []
with open('meseeks_new.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:

        z.append(float(row[0]))
        theta.append(float(row[1]))
        rho.append(float(row[2]))

z = [zee*4*0.32 for zee in z]
theta = [thee*0.03141592653 for thee in theta]
rho = [73.2 - rhee for rhee in rho]
x2 = [50,50,-50,-50,50,50,-50,-50]
y2 = [50,-50,50,-50,50,-50,50,-50]
z2 = [0,0,0,0,100,100,100,100]
def pol2cart(rh, th):
     x1 = rh* np.cos(th)
     y1 = rh * np.sin(th)
     return(x1,y1)
(x1, y1) = pol2cart(rho,theta)

a = np.asarray([ x1, y1, z ])
a = a.T
np.savetxt("meseeks_new.txt", a, delimiter=",")

fig = plt.figure(figsize=(8,8),dpi=100)
ax1 = fig.add_subplot(111, projection='3d')
ax1.scatter(x1,y1,z)
#plt.hold(True)
#ax2 = fig.add_subplot(111, projection='3d')
ax1.scatter(x2,y2,z2,c='w')
plt.show()


