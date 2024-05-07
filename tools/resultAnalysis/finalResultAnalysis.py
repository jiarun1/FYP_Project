import numpy as np
import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
from mpl_toolkits.mplot3d import Axes3D
from scipy.ndimage import gaussian_filter
from mpl_toolkits.axes_grid1.inset_locator import inset_axes


################ Data reading ##############

# # read the first set of data
# data_set = pd.read_csv("../../tests/mazeMapTest/V10_Code_Test.csv")
data_set = pd.read_csv("../../tests/squareMapTest/V3_3_1_Code_Test.csv")

# minimum_distance = np.sqrt(np.square(2)+np.square(8)) + \
#                    2 + \
#                    np.sqrt(np.square(2)+np.square(2)) + \
#                    4 + \
#                    np.sqrt(np.square(2)+np.square(2)) + \
#                    2 + \
#                    np.sqrt(np.square(2)+np.square(6)) + \
#                    2 + \
#                    np.sqrt(np.square(2)+np.square(8))


minimum_distance = 20 * np.sqrt(2)

# Datas
area_Set = data_set.loc[:,'Area Set'].values
angle_Set = data_set.loc[:,'Angle Set'].values
results = data_set.loc[:,'Result Distance'].values / minimum_distance * 100
points_num = data_set.loc[:,'Points Num'].values
triangle_time = data_set.loc[:,'Mapping Time(us)'].values
conversion_time = data_set.loc[:,'Convertion Time(us)'].values
pathplanning_time = data_set.loc[:,'Shorest Path Time(us)'].values
is_conforming = data_set.loc[:,'If Conforming'].values



area_data = sorted(list(set(area_Set)))
area_data = [x for x in area_data if x < 5]
angle_data = sorted(list(set(angle_Set)))
angle_data = [x for x in angle_data if x < 30]
results_data = np.zeros([len(area_data), len(angle_data)])
results_data_conforming = np.zeros([len(area_data), len(angle_data)])
points_num_data = np.zeros([len(area_data), len(angle_data)])
points_num_data_conforming  = np.zeros([len(area_data), len(angle_data)])
triangle_time_data = np.zeros([len(area_data), len(angle_data)])
triangle_time_data_conforming = np.zeros([len(area_data), len(angle_data)])
conversion_time_data = np.zeros([len(area_data), len(angle_data)])
conversion_time_data_conforming = np.zeros([len(area_data), len(angle_data)])
pathplanning_time_data = np.zeros([len(area_data), len(angle_data)])
pathplanning_time_data_conforming = np.zeros([len(area_data), len(angle_data)])
# convert the data into several type

print(angle_data)
print(len(area_Set))

for data_count in range(len(area_Set)):
    if area_Set[data_count] in area_data:
        x_loc = area_data.index(area_Set[data_count])
    else:
        continue
    if angle_Set[data_count] in angle_data:
        y_loc = angle_data.index(angle_Set[data_count])
    else:
        continue
    
    if(is_conforming[data_count] == 1):
        results_data_conforming[x_loc, y_loc] = results[data_count]
        points_num_data_conforming[x_loc, y_loc] = points_num[data_count]
        triangle_time_data_conforming[x_loc, y_loc] = triangle_time[data_count]
        conversion_time_data_conforming[x_loc, y_loc] = conversion_time[data_count]
        pathplanning_time_data_conforming[x_loc, y_loc] = pathplanning_time[data_count]
    else:
        results_data[x_loc, y_loc] = results[data_count]
        points_num_data[x_loc, y_loc] = points_num[data_count]
        triangle_time_data[x_loc, y_loc] = triangle_time[data_count]
        conversion_time_data[x_loc, y_loc] = conversion_time[data_count]
        pathplanning_time_data[x_loc, y_loc] = pathplanning_time[data_count]
        


###### Area and angle (x,y axis)
Area, Angle = np.meshgrid(area_data, angle_data)
Area_log = np.log10(Area)

###### final result distance
# results_data = np.nan_to_num(results_data, nan=0)
# results_data_conforming = np.nan_to_num(results_data_conforming, nan=0)
Result = results_data.T
Result_conforming = results_data_conforming.T
Result_smooth = gaussian_filter(Result, sigma=3)  # Adjust sigma as needed
Result_smooth_conforming = gaussian_filter(Result_conforming, sigma=3)  # Adjust sigma as needed

print(results_data)
result_min = min([min(min(row) for row in Result_smooth), min(min(row) for row in Result_smooth_conforming)])
result_max = max([max(max(row) for row in Result_smooth), max(max(row) for row in Result_smooth_conforming)])

print(result_min)
print(result_max)

result_min = 104
result_max = 107.5



###### Point Num Generated
PointsNum = points_num_data.T
PointsNum_conforming = points_num_data_conforming.T
PointsNum_log = np.log10(PointsNum)
PointsNum_log_conforming = np.log10(PointsNum_conforming)


PointNum_min = 0
PointNum_max = 175000

###### Triangle Time
TriangleTime = triangle_time_data.T
TriangleTime_conforming = triangle_time_data_conforming.T
TriangleTime_log = np.log10(TriangleTime)
TriangleTime_log_conforming = np.log10(TriangleTime_conforming)


TriangleTime_min = 0
TriangleTime_max = 750000

###### Total Time


total_time_data = triangle_time_data + conversion_time_data + pathplanning_time_data
total_time_data_conforming = triangle_time_data_conforming + conversion_time_data_conforming + pathplanning_time_data_conforming


TotalTime = total_time_data.T
TotalTime_conforming = total_time_data_conforming.T
TotalTime_log = np.log10(TotalTime)
TotalTime_log_conforming = np.log10(TotalTime_conforming)

# TriangleTime_min = 0
# TriangleTime_max = 750000

# print(results_data)

# print("xdata_size", Area.shape[0], Area.shape[1])
# print("ydata_size", Angle.shape[0], Angle.shape[1])
# print("zdata_size", results_data.shape[0], results_data.shape[1])

#########################
## Plot:

# Plot Setup
plot.rc('font',family='Arial')


##########################################################################################
# Plot 7 mapping area & constrain with result (2D)
###### Remove all nan and -inf data

# # mapping area vs point fitting

######### Data processing
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

x_data = Area_log[1,:]
y1_data = Result[1,:]
y2_data = Result_conforming[1,:]

Area_log_Average, Result_average, Result_Std = findAverage(x_data, y1_data, 1e-1)
Area_log_Average_conforming, Result_average_conforming, Result_Std_conforming = findAverage(x_data, y2_data, 1e-1)



def plot_1_display():
    fig = plot.figure("Mapping Area Vs Average Result",figsize=(9/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)

    # line1_handle = ax.scatter(x_data, y1_data,label="Without Conforming",s=1, color = "#6CB0D6")
    ax.plot(Area_log_Average, Result_average, color="#0D4A70",label = "Average Results")
    # line3_handle = ax.fill_between(Area_log_Average, np.array(Result_average) - Result_Std, np.array(Result_average) + Result_Std, color="#0D4A70", alpha=0.2, label='Standard Deviation')
    ax.fill_between([-10,10], 0, 104, color="#0D4A70", alpha=0.2, label='Specification Range')

    plot.legend(ncol=1, loc = 'upper left')
    plot.xlabel('Max area settings',size=11)
    plot.ylabel('Minimum Distance (%)',size=11)

    ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    ax.set_xlim([-2.75,0.75])
    ax.set_ylim([101.5,109.5])
    plot.grid()
    plot.tight_layout()


def plot_2_display():
    fig = plot.figure("Mapping Area Vs Average STD",figsize=(9/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)

    ax.plot(Area_log_Average, Result_Std, color="#0D4A70", label='Standard Deviation')
    ax.fill_between([-10,10], -10, 0.2, color="#0D4A70", alpha=0.2, label='Specification Range')



    # plot.legend(ncol=3, loc = 'upper left')
    plot.legend(ncol=1, loc = 'upper left')
    plot.xlabel('Max area settings',size=11)
    plot.ylabel('Minimum Distance (%)',size=11)

    ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    ax.set_xlim([-2.75,0.75])
    ax.set_ylim([-0.125,2])
    plot.grid()
    plot.tight_layout()



##### Plot 10
def area_vs_total_log_fitting(x, a, b): # function for the fitting
    return a*x + b

angle_0_index = angle_data.index(0)
angle_20_index = angle_data.index(20)
area_small_0_index = [i for i, v in enumerate(Area_log[angle_0_index,:]) if v < 0]
a,b = op.curve_fit(area_vs_total_log_fitting, Area_log[angle_0_index,area_small_0_index].T, TriangleTime_log[angle_0_index,area_small_0_index].T)[0]
TriangleTime_log_fit = [area_vs_triangleTime_log_fitting(x,a,b) for x in Area_log[angle_0_index,:].T]
a,b = op.curve_fit(area_vs_total_log_fitting, Area_log[angle_0_index,area_small_0_index].T, TriangleTime_log_conforming[angle_0_index,area_small_0_index].T)[0]
TriangleTime_log_conforming_fit = [area_vs_triangleTime_log_fitting(x,a,b) for x in Area_log[angle_0_index,:].T]
# area_pointNum_fit_function = str((a))+"/x+" +str(b)
# print("Mapping area vs point information:", area_pointNum_fit_function)

def plot_10_display():
    fig = plot.figure("Mapping Area Vs Triangle Time",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    ax.scatter(Area_log[angle_0_index,:], TriangleTime_log[angle_0_index,:], label = "Without Conforming", color = "#6CB0D6", s = 1)
    ax.plot(Area_log[angle_0_index,:], TriangleTime_log_fit, label = "Without Conforming Fitting", color = "#0D4A70")


    ax.scatter(Area_log[angle_0_index,:], TriangleTime_log_conforming[angle_0_index,:], label = "With Conforming", color = "#FD8D3C", s = 1)
    ax.plot(Area_log[angle_0_index,:], TriangleTime_log_conforming_fit, label = "With Conforming Fitting", color = "#810026")
    ax.legend()

    plot.xlabel('Max area settings',size=11)
    plot.ylabel('Triangle Time ($dB \mu s$)',size=11)

    ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    plot.grid()
    plot.tight_layout()



##### Plot 11
#TODO: add the code for angle settings vs triangle time
def angle_vs_triangleTime_log_fitting(x, a, b): # function for the fitting
    return a*x + b


# area_data_0_3 = area_data.index(0.300201)
a,b = op.curve_fit(angle_vs_triangleTime_log_fitting, Angle[:,area_data_0_3].T, TriangleTime_log[:,area_data_0_3].T)[0]
TriangleTime_log_fit_angle = [angle_vs_triangleTime_log_fitting(x,a,b) for x in Angle[:,area_data_0_3].T]
a,b = op.curve_fit(angle_vs_triangleTime_log_fitting, Angle[:,area_data_0_3].T, TriangleTime_log_conforming[:,area_data_0_3].T)[0]
TriangleTime_log_conforming_fit_angle = [angle_vs_triangleTime_log_fitting(x,a,b) for x in Angle[:,area_data_0_3].T]

def plot_11_display():
    fig = plot.figure("Mapping Angle Vs Triangle Time",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    ax.plot(Angle[:,area_data_0_3], TriangleTime_log[:,area_data_0_3], label = "Without Conforming", color = "#6CB0D6")#, s = 3)
    # ax.plot(Angle[:,area_data_0_3], TriangleTime_log_fit_angle, label = "Without Conforming Fitting", color = "#0D4A70")

    ax.plot(Angle[:,area_data_0_3], TriangleTime_log_conforming[:,area_data_0_3], label = "With Conforming", color = "#FD8D3C")#, s = 3)
    # ax.plot(Angle[:,area_data_0_3], TriangleTime_log_conforming_fit_angle, label = "With Conforming Fitting", color = "#810026")
    ax.legend()

    plot.xlabel('Mapping Angle Settings',size=11)
    plot.ylabel('Triangle Time ($dB \mu s$)',size=11)

    plot.grid()
    plot.tight_layout()


######## Plot 12
# Triangle Time vs Point number
def area_vs_triangleTime_log_fitting(x, a, b): # function for the fitting
    return a*x + b


point_small_2_index = [i for i, v in enumerate(PointsNum_log[angle_0_index,:]) if v > 3]
a,b = op.curve_fit(area_vs_triangleTime_log_fitting, PointsNum_log[angle_0_index,point_small_2_index].T, TriangleTime_log[angle_0_index,point_small_2_index].T)[0]
TriangleTime_log_fit_points_log = [area_vs_triangleTime_log_fitting(x,a,b) for x in PointsNum_log[angle_0_index,:].T]
TriangleTime_log_fit_points_log_fun =  '$' +'{:.4}'.format(a)+"x " + '{:+.4}'.format(b)+ '$'
a,b = op.curve_fit(area_vs_triangleTime_log_fitting, PointsNum_log[angle_0_index,point_small_2_index].T, TriangleTime_log_conforming[angle_0_index,point_small_2_index].T)[0]
TriangleTime_log_conforming_fit_points_log = [area_vs_triangleTime_log_fitting(x,a,b) for x in PointsNum_log[angle_0_index,:].T]



def plot_12_display():
    print(TriangleTime_log_fit_points_log_fun)

    fig = plot.figure("Points Number Vs Triangle Time Log",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    ax.scatter(PointsNum_log[angle_0_index,:], TriangleTime_log[angle_0_index,:], s= 1, label = "Without Conforming", color = "#6CB0D6")#, s = 3)
    ax.plot(PointsNum_log[angle_0_index,:], TriangleTime_log_fit_points_log, label = "Without Conforming Fitting", color = "#0D4A70")
    # ax.scatter(PointsNum_log[angle_20_index,:], TriangleTime_log[angle_20_index,:], s= 1, label = "Without Conforming", color = "#6CB0D6")#, s = 3)
 
    ax.scatter(PointsNum_log[angle_0_index,:], TriangleTime_log_conforming[angle_0_index,:],s= 1,  label = "With Conforming", color = "#FD8D3C")#, s = 3)
    ax.plot(PointsNum_log[angle_0_index,:], TriangleTime_log_conforming_fit_points_log, label = "With Conforming Fitting", color = "#810026")
    # ax.scatter(PointsNum_log[angle_20_index,:], TriangleTime_log_conforming[angle_20_index,:],s= 1,  label = "With Conforming", color = "#FD8D3C")#, s = 3)
    
    ax.legend()

    plot.xlabel('Points number (dB)',size=11)
    plot.ylabel('Triangle Time ($dB \mu s$)',size=11)

    plot.grid()
    plot.tight_layout()


def plot_12_1_display():
    fig = plot.figure("Points Number Vs Triangle Time (With and Without Conforming)",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)

    for i in range(20):
        
        angle_i_index = angle_data.index(i)
        ax.scatter(PointsNum_log[angle_i_index,:], TriangleTime_log[angle_i_index,:], s= 1, label = "Without Conforming", color = "#6CB0D6")#, s = 3)
        ax.scatter(PointsNum_log[angle_i_index,:], TriangleTime_log_conforming[angle_i_index,:],s= 1,  label = "With Conforming", color = "#FD8D3C")#, s = 3)

    
    
    # ax.legend()

    plot.xlabel('Points number (dB)',size=11)
    plot.ylabel('Triangle Time ($dB \mu s$)',size=11)

    plot.grid()
    plot.tight_layout()



######## Plot 13
# Triangle Time vs Point number
def pointsNum_vs_triangleTime_fitting(x, a, b, c): # function for the fitting
    return a*x*np.log10(x) + b * x + c


# point_small_2_index = [i for i, v in enumerate(PointsNum_log[angle_0_index,:]) if v > 3]
a,b,c = op.curve_fit(pointsNum_vs_triangleTime_fitting, PointsNum[angle_0_index,point_small_2_index].T, TriangleTime[angle_0_index,point_small_2_index].T)[0]
TriangleTime_fit_points = [pointsNum_vs_triangleTime_fitting(x,a,b,c) for x in PointsNum[angle_0_index,:].T]
TriangleTime_fit_points_fun =  '$' +'{:.4}'.format(a)+"x " + '{:+.4}'.format(b) + "x" + '{:+.4}'.format(c) + '$'
a,b,c = op.curve_fit(pointsNum_vs_triangleTime_fitting, PointsNum[angle_0_index,point_small_2_index].T, TriangleTime_conforming[angle_0_index,point_small_2_index].T)[0]
TriangleTime_conforming_fit_points = [pointsNum_vs_triangleTime_fitting(x,a,b,c) for x in PointsNum[angle_0_index,:].T]
TriangleTime_conforming_fit_points_fun =  '$' +'{:.4}'.format(a)+"x " + '{:+.4}'.format(b) + "x" + '{:+.4}'.format(c) + '$'


def plot_13_display():
    print(TriangleTime_fit_points_fun)
    print(TriangleTime_conforming_fit_points_fun)

    fig = plot.figure("Points Number Vs Triangle Time",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    ax.scatter(PointsNum[angle_0_index,:], TriangleTime[angle_0_index,:], s= 1, label = "Without Conforming", color = "#6CB0D6")#, s = 3)
    # ax.plot(PointsNum[angle_0_index,:], TriangleTime_fit_points, label = "Without Conforming Fitting", color = "#0D4A70")
    ax.plot(PointsNum[angle_0_index,:], TriangleTime_fit_points, color = "#0D4A70")
    # ax.scatter(PointsNum_log[angle_20_index,:], TriangleTime_log[angle_20_index,:], s= 1, label = "Without Conforming", color = "#6CB0D6")#, s = 3)
 
    ax.scatter(PointsNum[angle_0_index,:], TriangleTime_conforming[angle_0_index,:],s= 1,  label = "With Conforming", color = "#FD8D3C")#, s = 3)
    # ax.plot(PointsNum[angle_0_index,:], TriangleTime_conforming_fit_points, label = "With Conforming Fitting", color = "#810026")
    ax.plot(PointsNum[angle_0_index,:], TriangleTime_conforming_fit_points, color = "#810026")
    # ax.scatter(PointsNum_log[angle_20_index,:], TriangleTime_log_conforming[angle_20_index,:],s= 1,  label = "With Conforming", color = "#FD8D3C")#, s = 3)
    
    ax.legend()

    plot.xlabel('Points number',size=11)
    plot.ylabel('Triangle Time ($ \mu s$)',size=11)

    plot.grid()
    plot.tight_layout()

if __name__ == "__main__":
    plot_1_display()
    plot_2_display()
    # plot_3_display()
    # plot_4_display()
    # plot_5_display()
    # plot_6_display()
    # plot_7_display()
    # plot_8_display()
    # plot_9_display()
    # plot_10_display()
    # plot_11_display()

    plot.show()