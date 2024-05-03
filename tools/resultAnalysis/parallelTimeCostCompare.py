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

# read the first set of data
data_results = [pd.read_csv("../../tests/largeSquareMapTest/V3_3_1_Code_Test.csv"),
                pd.read_csv("../../tests/largeSquareMapTest/V6_3_1_Code_Test.csv")]

data_titles = ["Normal Dijkstra's",
               "Bidirectional Dijkstra's"]

data_colors = ["#6CB0D6",
               "#FD8D3C"]

data_colors2 = ["#0D4A70",
                "#810026"]


def data_1_Fit(x, a, b):
    return a*x*np.log10(x) + b
def data_2_Fit(x, a, b):
    return a*x*x + b

data_fit_functions = [data_1_Fit, 
                      data_1_Fit]


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
    shortest_path_time = current_data_set.loc[:,'Shorest Path Time(us)'].values
    is_conforming = current_data_set.loc[:,'If Conforming'].values

    area_data = sorted(list(set(area_Set)))
    angle_data = sorted(list(set(angle_Set)))

    # NOTE: all x axis data with fixed y has the same area settings 

    current_data["area"], current_data["angle"] = np.meshgrid(area_data, angle_data)
    current_data["area_log"] = np.log10(current_data["area"])
    current_data["size"] = [len(angle_data),len(area_data)]

    current_data["results"] = np.zeros(current_data["size"])
    current_data["results_conforming"] = np.zeros(current_data["size"])
    current_data["shortest_path_time"] = np.zeros(current_data["size"])
    current_data["points_num"] = np.zeros(current_data["size"])


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
            current_data["shortest_path_time"][x_loc, y_loc] = shortest_path_time[data_count]
            current_data["points_num"][x_loc, y_loc] = points_num[data_count]

    
    # print(current_data["results"][:,1])

    Datas.append(current_data)

# sys.exit()

print("data read successful")


for i in range(len(data_results)):
    current_data = Datas[i]

    ###### Constant Angle
    setted_angle = 0
    interval = 1e-1
    setted_angle_index = list(current_data["angle"][:,1]).index(setted_angle)
    points_num_range = [5,8e4]

    current_data["area_log_settled"] = current_data["area_log"][setted_angle_index,:]
    current_data["shortest_path_time_settled"] = current_data["shortest_path_time"][setted_angle_index,:]
    current_data["shortest_path_time_settled_log"] = np.log10(current_data["shortest_path_time_settled"])
    current_data["points_num_settled"] = current_data["points_num"][setted_angle_index,:]
    current_data["points_num_settled_log"] = np.log10(current_data["points_num_settled"])

    x_tmp = [x for x in current_data["points_num_settled"] if points_num_range[0] < x < points_num_range[1]]
    y_tmp = [current_data["shortest_path_time_settled"][i] for i in range(len(current_data["points_num_settled"])) if points_num_range[0] < current_data["points_num_settled"][i] < points_num_range[1]]

    # a, b = op.curve_fit(data_fit_functions[i], current_data["points_num_settled"], current_data["shortest_path_time_settled"])[0]

    a, b = op.curve_fit(data_fit_functions[i], x_tmp, y_tmp)[0]
    current_data["shortest_path_time_vs_points_fit"] = [data_fit_functions[i](x, a, b) for x in current_data["points_num_settled"]]
    current_data["shortest_path_time_vs_points_fit_fun"] = "$" + "{0:.2f}".format(a)+ "xlog(x) + " + "{0:.2f}".format(b) + "$"
    print(current_data["shortest_path_time_vs_points_fit_fun"])


# ###########################
##### Plot 1 for average
# Plot Setup
def plot_1():

    #### plot in average with percentage
    fig = plot.figure("Paralle compare",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    

    # print(Datas[0]["result_average"])

    for i in range(len(data_results)):
        plot.scatter(Datas[i]["points_num_settled"], Datas[i]["shortest_path_time_settled"], label = data_titles[i], color = data_colors[i], s = 3)
        plot.plot(Datas[i]["points_num_settled"], Datas[i]["shortest_path_time_vs_points_fit"], label = Datas[i]["shortest_path_time_vs_points_fit_fun"], color = data_colors2[i] )


    # plot.title('Average path length differences with different extra points',size=11)
    plot.legend()
    plot.xlabel('Number of Points',size=11)
    plot.ylabel('Cost Time ($ \mu s$)',size=11)
    plot.grid()

    # ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    # ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    ax.set_xlim([0,3e4])
    ax.set_ylim([0,8e4])
    plot.tight_layout()

# ###########################
##### Plot 1 for average
# Plot Setup
def plot_2():

    #### plot in average with percentage
    fig = plot.figure("Paralle compare in log",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    

    # print(Datas[0]["result_average"])

    for i in range(len(data_results)):
        plot.scatter(Datas[i]["points_num_settled_log"], Datas[i]["shortest_path_time_settled_log"], label = data_titles[i], color = data_colors[i], s = 3)


    # plot.title('Average path length differences with different extra points',size=11)
    plot.legend()
    plot.xlabel('Number of Points',size=11)
    plot.ylabel('Cost Time ($ \mu s$)',size=11)
    plot.grid()

    # ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    # ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    # ax.set_xlim([0,3e4])
    # ax.set_ylim([0,8e4])
    plot.tight_layout()


plot_1()
plot_2()
plot.show()