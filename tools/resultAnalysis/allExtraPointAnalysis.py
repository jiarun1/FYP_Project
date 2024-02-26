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


realDistance = 20*np.sqrt(2)

# read the first set of data
data_results = [pd.read_csv("../../tests/squareMapTest/V3_1_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_3_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_4_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_5_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_6_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_7_Code_Test.csv")]


data_titles = ["No extra point",
               "Fermat point",
               "Centriod",
               "Circumcenter",
               "Orthocenter",
               "Incenter"]

data_colors = ["#003f5c",
               "#2f4b7c",
               "#d45087",
               "#ffa600",
               "#777f8e",
               "#a05195"]



#########################
## Plot:

distances = list()
overalltimes = list()
for i in range(len(data_results)):
    distance_tmp = data_results[i].loc[:,'Result Distance'].values
    maptime_tmp  = data_results[i].loc[:,'Mapping Time(us)'].values
    convtime_tmp = data_results[i].loc[:,'Convertion Time(us)'].values
    pathtime_tmp = data_results[i].loc[:,'Shorest Path Time(us)'].values
    overalltime_tmp = maptime_tmp + convtime_tmp + pathtime_tmp
    distances.append(distance_tmp)
    overalltimes.append(overalltime_tmp)


######### Data processing
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

time_envelops = list()
distance_envelops = list()
distance_smooths = list()
distance_smooths_percentage = list()
for i in range(len(distances)):
    time_envelop_tmp , distance_envelop_tmp = findEnvelop(overalltimes[i], distances[i], 5e3)
    distance_envelop_smooth_tmp = savgol_filter(distance_envelop_tmp, 10, 3)
    time_envelops.append(time_envelop_tmp)
    distance_envelops.append(distance_envelop_tmp)
    distance_smooths.append(distance_envelop_smooth_tmp)
    distance_smooths_percentage.append(distance_envelop_smooth_tmp * 100 / realDistance)



# ###########################

# Plot Setup
plot.rc('font',family='Arial')

#### plot in envelop with percentage
plot.figure("Envelop of accuracy percentage",figsize=(21/2.54,9/2.54),dpi=200)

for i in range(len(time_envelops)):
    plot.plot(time_envelops[i], distance_smooths_percentage[i], label = data_titles[i], color = data_colors[i])

plot.title('Overall Cost Time Vs Performance',size=11)
plot.legend()
plot.xlabel('Overall Cost Time ($\mu s$)',size=11)
plot.ylabel('Accuracy Percentage (%)',size=11)
plot.grid()
plot.xlim([0,6e5])

plot.tight_layout()
plot.show()

wait = input("Press Enter to continue.")