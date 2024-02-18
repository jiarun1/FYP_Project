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
data_set_1 = pd.read_csv("../../tests/squareMapTest/V1_1_Code_Test.csv")
data_set_2 = pd.read_csv("../../tests/squareMapTest/V2_Code_Test.csv")
data_set_3 = pd.read_csv("../../tests/squareMapTest/V3_Code_Test.csv")

data_set_1_version_string = "Triditional"
data_set_2_version_string = "Fabonacci heap"
data_set_3_version_string = "Structure improvement"



#########################
## Plot:
# Datas
data_1_x = data_set_1.loc[:,'Points Num'].values
data_2_x = data_set_2.loc[:,'Points Num'].values
data_3_x = data_set_3.loc[:,'Points Num'].values
data_1_y = data_set_1.loc[:,'Shorest Path Time(us)'].values
data_2_y = data_set_2.loc[:,'Shorest Path Time(us)'].values
data_3_y = data_set_3.loc[:,'Shorest Path Time(us)'].values

######### NORMAL PLOT IN CARTESIAN
# Data Fitting Perform
def dataSet_1_Fit(x, a, b, c):
    return a*(np.square(x)) + b*x + c

def dataSet_2_Fit(x, a, b, c):
    return a*(np.square(x)) + b*x + c

def dataSet_3_Fit(x, a, b, c):
    return a*(np.square(x)) + b*x + c

# Data set 1 fitting
a_1,b_1,c_1 = op.curve_fit(dataSet_1_Fit, data_1_x, data_1_y)[0]
a_2,b_2,c_2 = op.curve_fit(dataSet_2_Fit, data_2_x, data_2_y)[0]
a_3,b_3,c_3 = op.curve_fit(dataSet_3_Fit, data_3_x, data_3_y)[0]

data_1_fitResult = [dataSet_1_Fit(x,a_1,b_1,c_1) for x in data_1_x]
data_2_fitResult = [dataSet_2_Fit(x,a_2,b_2,c_2) for x in data_2_x]
data_3_fitResult = [dataSet_3_Fit(x,a_3,b_3,c_3) for x in data_3_x]

data_1_fitFunction =  str(a_1)+"x^2 +" + str(b_1) + "x+" + str(c_1)
data_2_fitFunction =  str(a_2)+"x^2 +" + str(b_2) + "x+" + str(c_2)
data_3_fitFunction =  str(a_3)+"x^2 +" + str(b_3) + "x+" + str(c_3)

print("Data 1: " , data_1_fitFunction)
print("Data 2: " , data_2_fitFunction)
print("Data 3: " , data_3_fitFunction)

###########################
set_1_defaultColor = ["#6CB0D6", "#0D4A70"]
set_2_defaultColor = ["#FD8D3C", "#810026"]
set_3_defaultColor = ['#b49e89', '#47254b']

# Plot Setup
plot.rc('font',family='Arial')
#plot.ion()
plot.figure("Mapping Plot",figsize=(21/2.54,9/2.54),dpi=200)
# Data Set 1
plot.scatter(data_1_x,data_1_y, label = data_set_1_version_string + " Test Result", s = 3, color = set_1_defaultColor[0])
plot.plot(data_1_x, data_1_fitResult, label = data_set_1_version_string + " Curve Fitted Result",color = set_1_defaultColor[1])

# Data Set 2
plot.scatter(data_2_x,data_2_y, label = data_set_2_version_string + " Test Result", s = 3, color = set_2_defaultColor[0])
plot.plot(data_2_x, data_2_fitResult, label = data_set_2_version_string + " Curve Fitted Result",color = set_2_defaultColor[1])

# Data Set 3
plot.scatter(data_3_x,data_3_y, label = data_set_3_version_string + " Test Result", s = 3, color = set_3_defaultColor[0])
plot.plot(data_3_x, data_3_fitResult, label = data_set_3_version_string + " Curve Fitted Result",color = set_3_defaultColor[1])

plot.title('Point Number Vs Shortest Path Execution Time',size=11)
plot.legend(prop={'size': 10})
plot.xlabel('Point Number',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()
plot.xlim([0,3e4])
plot.ylim([0,5e6])

#################### PLOT IN LOGLOG
data_1_log_x = np.log10(data_1_x)
data_1_log_y = np.log10(data_1_y)
data_2_log_x = np.log10(data_2_x)
data_2_log_y = np.log10(data_2_y)
data_3_log_x = np.log10(data_3_x)
data_3_log_y = np.log10(data_3_y)

# Data Fitting Perform
def dataSet_1_logFit(x, a, b):
    return a*x + b

def dataSet_2_logFit(x, a, b):
    return a*x + b

def dataSet_3_logFit(x, a, b):
    return a*x + b


# Data set 1 fitting
a_1_log,b_1_log = op.curve_fit(dataSet_1_logFit, data_1_log_x, data_1_log_y)[0]
a_2_log,b_2_log = op.curve_fit(dataSet_2_logFit, data_2_log_x, data_2_log_y)[0]
a_3_log,b_3_log = op.curve_fit(dataSet_3_logFit, data_3_log_x, data_3_log_y)[0]

data_1_log_fitResult = [dataSet_1_logFit(x,a_1_log,b_1_log) for x in data_1_log_x]
data_2_log_fitResult = [dataSet_2_logFit(x,a_2_log,b_2_log) for x in data_2_log_x]
data_3_log_fitResult = [dataSet_3_logFit(x,a_3_log,b_3_log) for x in data_3_log_x]

data_1_log_fitFunction =  '$' +'{:.2}'.format(a_1_log)+"x " + '{:+.2}'.format(b_1_log)+ '$'
data_2_log_fitFunction =  '$' +'{:.2}'.format(a_2_log)+"x " + '{:+.2}'.format(b_2_log)+ '$'
data_3_log_fitFunction =  '$' +'{:.2}'.format(a_3_log)+"x " + '{:+.2}'.format(b_3_log)+ '$'

###########################

# Plot Setup
plot.figure("Mapping Plot in Loglog",figsize=(21/2.54,9/2.54),dpi=200)
# # Data Set 1
# plot.scatter(data_1_log_x,data_1_log_y, label= data_set_1_version_string + " test result", s = 3, color = set_1_defaultColor[0])
# plot.plot(data_1_log_x, data_1_log_fitResult, label = data_1_log_fitFunction,color = set_1_defaultColor[1])

# # Data Set 2
# plot.scatter(data_2_log_x,data_2_log_y, label= data_set_2_version_string + " test result" , s = 3, color = set_2_defaultColor[0])
# plot.plot(data_2_log_x, data_2_log_fitResult, label = data_2_log_fitFunction,color = set_2_defaultColor[1])

# # Data Set 3
# plot.scatter(data_3_log_x,data_3_log_y, label= data_set_3_version_string + " test result" , s = 3, color = set_3_defaultColor[0])
# plot.plot(data_3_log_x, data_3_log_fitResult, label = data_3_log_fitFunction,color = set_3_defaultColor[1])


# Data Set 1
plot.scatter(data_1_log_x,data_1_log_y, label= data_set_1_version_string + " test result", s = 3, color = set_1_defaultColor[0])
plot.plot(data_1_log_x, data_1_log_fitResult,color = set_1_defaultColor[1])

# Data Set 2
plot.scatter(data_2_log_x,data_2_log_y, label= data_set_2_version_string + " test result" , s = 3, color = set_2_defaultColor[0])
plot.plot(data_2_log_x, data_2_log_fitResult,color = set_2_defaultColor[1])

# Data Set 3
plot.scatter(data_3_log_x,data_3_log_y, label= data_set_3_version_string + " test result" , s = 3, color = set_3_defaultColor[0])
plot.plot(data_3_log_x, data_3_log_fitResult,color = set_3_defaultColor[1])

plot.title('Point Number Vs Shortest Path Execution Time',fontsize=10)
plot.legend(fontsize=10)
plot.xlabel('Point Number (dB)',fontsize=10)
plot.ylabel('Cost time ($ dB\mu s$)',fontsize=10)
plot.grid()

plot.xlim([1.5,4.5])
plot.ylim([1, 6.5])

plot.tight_layout()
plot.show()

wait = input("Press Enter to continue.")