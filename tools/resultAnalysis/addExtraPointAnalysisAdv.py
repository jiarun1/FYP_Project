import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
from scipy.signal import savgol_filter
import tkinter as tk
from tkinter.filedialog import askopenfilename
import sys

from scipy.ndimage import gaussian_filter


#========================= Changed Data Area ========================================
# Data Processing
# data class


realDistance = 20*np.sqrt(2)
map_area = 20*20

# read the first set of data
data_results = [pd.read_csv("../../tests/squareMapTest/V3_3_1_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_3_2_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_4_1_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_5_1_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_6_1_Code_Test.csv"),
                pd.read_csv("../../tests/squareMapTest/V4_7_2_Code_Test.csv")]


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

########################
# Data Readings
Datas = list()


for i in range(len(data_results)):
    current_data_set = data_results[i]
    current_data = {}
    
    area_Set = current_data_set.loc[:,'Area Set'].values
    angle_Set = current_data_set.loc[:,'Angle Set'].values
    results = current_data_set.loc[:,'Result Distance'].values / realDistance * 100 - 100
    points_num = current_data_set.loc[:,'Points Num'].values
    triangle_time = current_data_set.loc[:,'Mapping Time(us)'].values
    is_conforming = current_data_set.loc[:,'If Conforming'].values

    area_data = sorted(list(set(area_Set)))
    angle_data = sorted(list(set(angle_Set)))

    # NOTE: all x axis data with fixed y has the same area settings 

    current_data["area"], current_data["angle"] = np.meshgrid(area_data, angle_data)
    current_data["area_log"] = np.log10(current_data["area"])
    current_data["size"] = [len(angle_data),len(area_data)]

    current_data["results"] = np.zeros(current_data["size"])
    current_data["results_conforming"] = np.zeros(current_data["size"])

    for data_count in range(len(area_Set)):
        if area_Set[data_count] in area_data:
            y_loc = area_data.index(area_Set[data_count])
        else:
            continue
        if angle_Set[data_count] in angle_data:
            x_loc = angle_data.index(angle_Set[data_count])
        else:
            continue
        
        if(is_conforming[data_count] == 1):
            current_data["results_conforming"][x_loc, y_loc] = results[data_count]
        else:
            current_data["results"][x_loc, y_loc] = results[data_count]

    
    # print(current_data["results"][:,1])

    Datas.append(current_data)

# sys.exit()

print("data read successful")

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

def findAverage(x, y, interval):
    x_modified = list()
    y_average = list()
    y_std = list()
    for i in range(int((max(x) - min(x))/interval)):
        mask = (x >= (i*interval + min(x))) & (x < ((i+1)*interval + min(x)))
        if((np.count_nonzero(mask)) == 0):
            continue
        x_range = x[mask]
        y_range = y[mask]
        

        x_modified.append((i+1)*interval + min(x))
        y_average.append(np.mean(y_range))
        y_std.append(np.std(y_range))

    return x_modified, y_average, y_std

def findAverage2(x, y, interval, x_range):
    x_modified = list()
    y_average = list()
    y_std = list()
    for i in range(int((max(x_range) - min(x_range))/interval)):
        mask = (x >= (i*interval + min(x_range))) & (x < ((i+1)*interval + min(x_range)))
        if((np.count_nonzero(mask)) == 0):
            continue
        x_range_tmp = x[mask]
        y_range_tmp = y[mask]
        

        x_modified.append((i+1)*interval + min(x_range))
        y_average.append(np.mean(y_range_tmp))
        y_std.append(np.std(y_range_tmp))

    return x_modified, y_average, y_std


for i in range(len(data_results)):
    current_data = Datas[i]

    ###### Constant Angle
    setted_angle = 0
    interval = 1e-1
    setted_angle_index = list(current_data["angle"][:,1]).index(setted_angle)
    current_data["area_log_average"], current_data["result_average"], current_data["result_std"] = findAverage(current_data["area_log"][setted_angle_index,:], current_data["results"][setted_angle_index,:], interval)
    current_data["area_log_average"], current_data["result_average_conforming"], current_data["results_std_conforming"] = findAverage(current_data["area_log"][setted_angle_index,:], current_data["results_conforming"][setted_angle_index,:], interval)


    current_data["result_std_smoothed"] = gaussian_filter(current_data["result_std"],1)
    current_data["result_average_smoothed"] = gaussian_filter(current_data["result_average"],1)

    setted_area = 0.00488609
    setted_area_index = list(current_data["area"][1,:]).index(setted_area)
    # current_data["angle_average"], current_data["angle_result_average"], current_data["angle_result_std"] = findAverage(current_data["angle"][setted_angle_index,:], current_data["results"][setted_angle_index,:], 1)
    current_data["angle_fix_area"]  = current_data["angle"][:,setted_area_index]
    current_data["result_fix_area"] = current_data["results"][:,setted_area_index]


    area_range_dB = [-3,0.5]
    current_data["area_log_average_in_range"], current_data["result_average_in_range"], current_data["result_std_in_range"] = findAverage2(current_data["area_log"][setted_angle_index,:], current_data["results"][setted_angle_index,:], interval,area_range_dB)


    current_data["result_std_smoothed_in_range"] = gaussian_filter(current_data["result_std_in_range"],1.5)
    current_data["result_average_smoothed_in_range"] = gaussian_filter(current_data["result_average_in_range"],0)
    


# ###########################
##### Plot 1 for average
# Plot Setup
def plot_1():

    #### plot in average with percentage
    fig = plot.figure("Average of accuracy percentage without conforming",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    

    # print(Datas[0]["result_average"])

    for i in range(len(data_results)):
        plot.plot(Datas[i]["area_log_average_in_range"] - np.log10(map_area), Datas[i]["result_average_smoothed_in_range"], label = data_titles[i], color = data_colors[i])

    # plot.title('Average path length differences with different extra points',size=11)
    plot.legend(loc=1)
    plot.xlabel('Max area / Map area (dB)',size=11)
    plot.ylabel('Path Length Differences (%)',size=11)
    plot.grid()

    # ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    # ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 

    plot.tight_layout()

# ###########################
##### Plot 2 for STD
# Plot Setup
def plot_2():
    plot.rc('font',family='Arial')

    #### plot in average with percentage
    fig = plot.figure("Standard divation of accuracy percentage without conforming",figsize=(21/2.54,9/2.54),dpi=200)

    ax = fig.add_subplot(111)

    for i in range(len(data_results)):
        plot.plot(Datas[i]["area_log_average_in_range"] - np.log10(map_area), Datas[i]["result_std_smoothed_in_range"], label = data_titles[i], color = data_colors[i])

    # plot.title('Area Settings Vs Performance Without Conforming',size=11)
    plot.legend()
    plot.xlabel('Max area / Map area (dB)',size=11)
    plot.ylabel('Path Length Differences (%)',size=11)
    plot.grid()



    # ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    # ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    # plot.xlim([0,6e5])

    plot.tight_layout()



# ###########################
##### Plot 3 for average
#### plot in Average with percentage
def plot_3():
    plot.figure("Average of accuracy percentage with conforming",figsize=(21/2.54,9/2.54),dpi=200)

    for i in range(len(data_results)):
        plot.plot(Datas[i]["area_log_average"] - np.log10(map_area), Datas[i]["result_average_conforming"], label = data_titles[i], color = data_colors[i])

    # plot.title('Area Settings Vs Performance With Conforming',size=11)
    plot.legend()
    plot.xlabel('Max area / Map area (dB)',size=11)
    plot.ylabel('Path Length Differences (%)',size=11)
    plot.grid()
    # plot.xlim([0,6e5])

    plot.tight_layout()


# ###########################
##### Plot 4 for average angle
#### plot in Average with percentage
def plot_4():

    plot.figure("Average of accuracy percentage with conforming (with angle)",figsize=(21/2.54,9/2.54),dpi=200)

    for i in range(len(data_results)):
        plot.plot(Datas[i]["angle_fix_area"], Datas[i]["result_fix_area"], label = data_titles[i], color = data_colors[i])


    print(Datas[5]["result_fix_area"])

    plot.title('Angle Settings Vs Performance',size=11)
    plot.legend()
    plot.xlabel('Angle ($^\circ$)',size=11)
    plot.ylabel('Path Length Differences (%)',size=11)
    plot.grid()
    # plot.xlim([0,30])

    plot.tight_layout()



plot.rc('font',family='Arial')

plot_1()
plot_2()
plot.show()