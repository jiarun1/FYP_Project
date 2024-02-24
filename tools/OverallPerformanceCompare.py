import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
from scipy.signal import savgol_filter
import tkinter as tk
from tkinter.filedialog import askopenfilename


#========================= Changed Data Area ========================================
# Data Processing
# data class

# read the first set of data
data_set_1 = pd.read_csv("../tests/squareMapTest/V4_3_Code_Test.csv")
data_set_2 = pd.read_csv("../tests/squareMapTest/V4_6_Code_Test.csv")

data_set_1_version_string = "No extra point"
data_set_2_version_string = "Circumcenter"

print(data_set_1.head)


#########################
## Plot:
realDistance = 20*np.sqrt(2)

# Datas
distance_1 = data_set_1.loc[:,'Result Distance'].values
mapTime_1  = data_set_1.loc[:,'Mapping Time(us)'].values
ConvTime_1 = data_set_1.loc[:,'Convertion Time(us)'].values
PathTime_1 = data_set_1.loc[:,'Shorest Path Time(us)'].values
distance_2 = data_set_2.loc[:,'Result Distance'].values
mapTime_2  = data_set_2.loc[:,'Mapping Time(us)'].values
ConvTime_2 = data_set_2.loc[:,'Convertion Time(us)'].values
PathTime_2 = data_set_2.loc[:,'Shorest Path Time(us)'].values

overallTime_1 = mapTime_1 + ConvTime_1 + PathTime_1
overallTime_2 = mapTime_2 + ConvTime_2 + PathTime_2





######### Data processing

# Data Fitting Perform
def dataSet_1_Fit(x, a, b):
    return a/x + b


def findEnvelop(x, y, interval):
    x_modified = list()
    y_modified = list()
    for i in range(int(max(x)/interval)):
        mask = (x >= (i*interval)) & (x < ((i+1)*interval))
        if((np.count_nonzero(mask)) == 0):
            continue
        x_range = x[mask]
        y_range = y[mask]
        max_index = np.argmax(y_range)

        x_modified.append(x_range[max_index])
        y_modified.append(y_range[max_index])

    return x_modified, y_modified


time_envelop_1, distance_envelop_1 = findEnvelop(overallTime_1, distance_1, 5e3)
time_envelop_2, distance_envelop_2 = findEnvelop(overallTime_2, distance_2, 5e3)

# Data set 1 fitting
a_1,b_1 = op.curve_fit(dataSet_1_Fit, time_envelop_1, distance_envelop_1)[0]
distance_envelop_1_fit = [dataSet_1_Fit(x,a_1,b_1) for x in time_envelop_1]
distance_envelop_1_smooth = savgol_filter(distance_envelop_1, 10, 3)

a_1,b_1 = op.curve_fit(dataSet_1_Fit, time_envelop_2, distance_envelop_2)[0]
distance_envelop_2_fit = [dataSet_1_Fit(x,a_1,b_1) for x in time_envelop_2]
distance_envelop_2_smooth = savgol_filter(distance_envelop_2, 10, 3)



# convert the data into percentage
distance_envelop_1_percentage = distance_envelop_1 / realDistance * 100
distance_envelop_2_percentage = distance_envelop_2 / realDistance * 100
distance_envelop_1_percentage_smooth = distance_envelop_1_smooth / realDistance * 100
distance_envelop_2_percentage_smooth = distance_envelop_2_smooth / realDistance * 100


###########################
set_1_defaultColor = ["#6CB0D6", "#0D4A70"]
set_2_defaultColor = ["#FD8D3C", "#810026"]

# Plot Setup
plot.rc('font',family='Arial')
#plot.ion()
plot.figure("Cost Time with distance",figsize=(21/2.54,9/2.54),dpi=200)
# Data Set 1
plot.scatter(overallTime_1,distance_1, label = data_set_1_version_string + "Test Result", s = 10, color = set_1_defaultColor[0])
# plot.plot(data_1_x, data_1_fitResult, label = data_set_1_version_string + "Curve Fitted Result",color = set_1_defaultColor[1])

# Data Set 2
plot.scatter(overallTime_2,distance_2, label = data_set_2_version_string + "Test Result", s = 10, color = set_2_defaultColor[0])
# plot.plot(data_2_x, data_2_fitResult, label = data_set_2_version_string + "Curve Fitted Result",color = set_2_defaultColor[1])

plot.title('Overall Cost Time Vs Result Distance',size=11)
plot.legend()
plot.xlabel('Overall Cost Time ($\mu s$)',size=11)
plot.ylabel('Result Distance',size=11)
plot.grid()
plot.xlim([0,6e5])


#### plot in envelop
plot.figure("Envelop of accuracy",figsize=(21/2.54,9/2.54),dpi=200)
# Data Set 1
plot.plot(time_envelop_1, distance_envelop_1, label = data_set_1_version_string + " Test Result", color = set_1_defaultColor[0])
plot.plot(time_envelop_1, distance_envelop_1_smooth, label = data_set_1_version_string + " Smooth Result", color = set_1_defaultColor[1])
# plot.plot(time_envelop_1, distance_envelop_1_fit, label = data_set_1_version_string + "Fit Result", color = set_1_defaultColor[1])

# Data Set 2
plot.plot(time_envelop_2, distance_envelop_2, label = data_set_2_version_string + " Test Result", color = set_2_defaultColor[0])
plot.plot(time_envelop_2, distance_envelop_2_smooth, label = data_set_2_version_string + " Smooth Result", color = set_2_defaultColor[1])
# plot.plot(time_envelop_2, distance_envelop_2_fit, label = data_set_2_version_string + "Fit Result", color = set_2_defaultColor[1])

plot.title('Overall Cost Time Vs Result Distance',size=11)
plot.legend()
plot.xlabel('Overall Cost Time ($\mu s$)',size=11)
plot.ylabel('Result Distance',size=11)
plot.grid()
plot.xlim([0,6e5])

#### plot in envelop with percentage
plot.figure("Envelop of accuracy percentage",figsize=(21/2.54,9/2.54),dpi=200)
# Data Set 1
plot.plot(time_envelop_1, distance_envelop_1_percentage, label = data_set_1_version_string + " Test Result", color = set_1_defaultColor[0])
plot.plot(time_envelop_1, distance_envelop_1_percentage_smooth, label = data_set_1_version_string + " Smooth Result", color = set_1_defaultColor[1])
# plot.plot(time_envelop_1, distance_envelop_1_fit, label = data_set_1_version_string + "Fit Result", color = set_1_defaultColor[1])

# Data Set 2
plot.plot(time_envelop_2, distance_envelop_2_percentage, label = data_set_2_version_string + " Test Result", color = set_2_defaultColor[0])
plot.plot(time_envelop_2, distance_envelop_2_percentage_smooth, label = data_set_2_version_string + " Smooth Result", color = set_2_defaultColor[1])
# plot.plot(time_envelop_2, distance_envelop_2_fit, label = data_set_2_version_string + "Fit Result", color = set_2_defaultColor[1])

plot.title('Overall Cost Time Vs Performance',size=11)
plot.legend()
plot.xlabel('Overall Cost Time ($\mu s$)',size=11)
plot.ylabel('Accuracy Percentage (%)',size=11)
plot.grid()
plot.xlim([0,6e5])



plot.tight_layout()
plot.show()

wait = input("Press Enter to continue.")