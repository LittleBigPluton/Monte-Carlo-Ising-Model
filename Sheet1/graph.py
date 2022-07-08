#!/usr/bin/python3.8
from pandas import read_csv
from matplotlib import pyplot
from numpy import array,shape,power

# In mean.dat file, data is ordered as rows instead of columns.
#   mean.dat file               how plotting function expects
# x1 y11 y12 y13 y14 ...        x1 y11 y21 y31 ...
# x2 y21 y22 y23 y24 ...        x2 y12 y22 y32 ...
# x3 y31 y32 y33 y34 ...        x3 y13 y23 y33 ...
# .   .   .   .   .  ...        .   .   .   .  ...
# .   .   .   .   .  ...        .   .   .   .  ...
#
# for i in range(0,len(x)):
#   scatter([x[i]]*(len(y)-1),y[i,:])
#Here X array's i'th value coppied as long as length of Y array so that X and Y have same length.
#During for loop, for each X array's i'th values, each values of Y array's i'th row is plotted.

#To apply a power law fit, define power function
def power_law(x, a, b):
    return a*x**b

pyplot.figure("Sample Size vs Means") #Create plotting environment
mean_data = array(read_csv("mean.dat",sep= " ",header=None, index_col = None)) #Read mean data from file
x = array(mean_data[:,0]) #Seperate X axis points from readed data file
y = array(mean_data[:,1:-1]) #Seperate Y axis points from readed data file
for i in range(0,len(x)):
    pyplot.scatter([x[i]]*(len(y[i])),y[i,:]) #Described at the beginning
pyplot.semilogx(basex=10) #Scale X-axis as logarithmic with base 10
pyplot.xlabel("Sample Size") #Set X label
pyplot.ylabel("Means") #Set Y label
pyplot.grid(which='major',linestyle='-.',linewidth=0.5) #Set major grid points
pyplot.grid(which='minor',linestyle=':',linewidth=1) #Set minor grid points
pyplot.savefig("MUvsN.pdf",format='pdf',orientation='landscape',dpi=1000)

pyplot.figure("Sample Size vs Deviation") #Create another plotting environment
deviation_data = array(read_csv("deviation.dat",header=None)) #Read data from *.dat file without header
params = array(read_csv("params.dat",header=None))
pyplot.loglog(x,deviation_data,'o',label="Standart Deviation") #Call plotting function. Loglog plots both x and y axes as log scale
pyplot.plot(x,power_law(x,*params),label="Power Fit: a = %1.5f and b= %1.5f" %tuple(params)) #Plot fitting line on the loglog plot
pyplot.ylabel("Sample Size") #Set Y label
pyplot.xlabel("Deviation of Average Values for M Tries") #Set X label
pyplot.grid(which='major',linestyle='-.',linewidth=1) #Set major grid point
pyplot.grid(which='minor',linestyle=':',linewidth=0.8) #Set minor grid points
pyplot.legend()
pyplot.savefig("SGMvsN.pdf",format='pdf',orientation='landscape',dpi=1000)
pyplot.show()
