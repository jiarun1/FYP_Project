import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
import tkinter as tk
from tkinter.filedialog import askopenfilename


#========================= Changed Data Area ========================================
# Data Processing
# data class

# read the first set of data
data_set_1 = pd.read_csv("../tests/squareMapTest/V1_Code_Test.csv")
data_set_2 = pd.read_csv("../tests/squareMapTest/V1_1_Code_Test.csv")

print(data_set_1.head)


#########################
## Plot:
# Datas
data_1_x = data_set_1.loc[:,'Points Num'].values
data_2_x = data_set_2.loc[:,'Points Num'].values
data_1_y = data_set_1.loc[:,'Shorest Path Time(us)'].values
data_2_y = data_set_2.loc[:,'Shorest Path Time(us)'].values

######### NORMAL PLOT IN CARTESIAN
# Data Fitting Perform
def dataSet_1_Fit(x, a, b, c):
    return a*(np.square(x)) + b*x + c

def dataSet_2_Fit(x, a, b, c):
    return a*(np.square(x)) + b*x + c

# Data set 1 fitting
a_1,b_1,c_1 = op.curve_fit(dataSet_1_Fit, data_1_x, data_1_y)[0]
a_2,b_2,c_2 = op.curve_fit(dataSet_2_Fit, data_2_x, data_2_y)[0]

data_1_fitResult = [dataSet_1_Fit(x,a_1,b_1,c_1) for x in data_1_x]
data_2_fitResult = [dataSet_2_Fit(x,a_2,b_2,c_2) for x in data_2_x]

data_1_fitFunction =  str(a_1)+"x^2 +" + str(b_1) + "x" + str(c_1)
data_2_fitFunction =  str(a_2)+"x^2 +" + str(b_2) + "x" + str(c_2)

###########################
set_1_defaultColor = ["#6CB0D6", "#0D4A70"]
set_2_defaultColor = ["#FD8D3C", "#810026"]

# Plot Setup
plot.rc('font',family='Arial')
plot.ion()
plot.figure("Mapping Plot",figsize=(21/2.54,9/2.54),dpi=200)
# Data Set 1
plot.scatter(data_1_x,data_1_y, label="V1 Test Result", s = 10, color = set_1_defaultColor[0])
plot.plot(data_1_x, data_1_fitResult, label = "V1 Curve Fitted Result",color = set_1_defaultColor[1])

# Data Set 2
plot.scatter(data_2_x,data_2_y, label="V2 Test Result", s = 10, color = set_2_defaultColor[0])
plot.plot(data_2_x, data_2_fitResult, label = "V2 Curve Fitted Result",color = set_2_defaultColor[1])

plot.title('Point Number Vs Shortest Path Execution Time',size=11)
plot.legend()
plot.xlabel('Point Number',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()

#################### PLOT IN LOGLOG
data_1_log_x = np.log10(data_1_x)
data_1_log_y = np.log10(data_1_y)
data_2_log_x = np.log10(data_2_x)
data_2_log_y = np.log10(data_2_y)

# Data Fitting Perform
def dataSet_1_logFit(x, a, b):
    return a*x + b

def dataSet_2_logFit(x, a, b):
    return a*x + b

# Data set 1 fitting
a_1_log,b_1_log = op.curve_fit(dataSet_1_logFit, data_1_log_x, data_1_log_y)[0]
a_2_log,b_2_log = op.curve_fit(dataSet_2_logFit, data_2_log_x, data_2_log_y)[0]

data_1_log_fitResult = [dataSet_1_logFit(x,a_1_log,b_1_log) for x in data_1_log_x]
data_2_log_fitResult = [dataSet_2_logFit(x,a_2_log,b_2_log) for x in data_2_log_x]

data_1_log_fitFunction =  '$' +'{:.2e}'.format(a_1_log)+"x " + '{:+.2e}'.format(b_1_log)+ '$'
data_2_log_fitFunction =  '$' +'{:.2e}'.format(a_2_log)+"x " + '{:+.2e}'.format(b_2_log)+ '$'

###########################
set_1_defaultColor = ["#6CB0D6", "#0D4A70"]
set_2_defaultColor = ["#FD8D3C", "#810026"]

# Plot Setup
plot.figure("Mapping Plot in Loglog",figsize=(21/2.54,9/2.54),dpi=200)
# Data Set 1
plot.scatter(data_1_log_x,data_1_log_y, label="V1 Test Result", s = 10, color = set_1_defaultColor[0])
plot.plot(data_1_log_x, data_1_log_fitResult, label = data_1_log_fitFunction,color = set_1_defaultColor[1])

# Data Set 2
plot.scatter(data_2_log_x,data_2_log_y, label="V1.1 Test Result", s = 10, color = set_2_defaultColor[0])
plot.plot(data_2_log_x, data_2_log_fitResult, label = data_2_log_fitFunction,color = set_2_defaultColor[1])

plot.title('Point Number Vs Shortest Path Execution Time',size=11)
plot.legend()
plot.xlabel('Point Number (dB)',size=11)
plot.ylabel('Cost time ($ dB $)',size=11)
plot.grid()













plot.tight_layout()
plot.show()

wait = input("Press Enter to continue.")