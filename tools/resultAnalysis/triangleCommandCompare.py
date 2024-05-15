import numpy as np
import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.axes_grid1.inset_locator import inset_axes
from scipy.ndimage import gaussian_filter


################ Data reading ##############

# read the first set of data
data_set = pd.read_csv("../../tests/squareMapTest/V3_3_Code_Test.csv")
# data_set = pd.read_csv("../../tests/RuggedMapTest/V6_3_1_Code_Test.csv")

minimum_distance = 20*np.sqrt(2)
map_area = 20*20

# Datas
area_Set = data_set.loc[:,'Area Set'].values
angle_Set = data_set.loc[:,'Angle Set'].values
results = data_set.loc[:,'Result Distance'].values / minimum_distance * 100 - 100
points_num = data_set.loc[:,'Points Num'].values
triangle_time = data_set.loc[:,'Mapping Time(us)'].values
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
    else:
        results_data[x_loc, y_loc] = results[data_count]
        points_num_data[x_loc, y_loc] = points_num[data_count]
        triangle_time_data[x_loc, y_loc] = triangle_time[data_count]
        


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

# print(results_data)

# print("xdata_size", Area.shape[0], Area.shape[1])
# print("ydata_size", Angle.shape[0], Angle.shape[1])
# print("zdata_size", results_data.shape[0], results_data.shape[1])

#########################
## Plot:

# Plot Setup
plot.rc('font',family='Arial')



##########################################################################################
# Plot 1 plot the minimum distance without Conforming (3D)
def plot_1_display():
    fig = plot.figure("Minimum Distance (No Conforming)",figsize=(10.5/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111,projection='3d')
    ax.plot_surface(Area_log - np.log10(map_area), Angle, Result_smooth, cmap='viridis',  vmin= result_min - 100, vmax= result_max - 100)


    # ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
    # ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

    ax.set_xlabel('Max area / Map area ($dB$)',fontsize=10)
    ax.set_ylabel('Angle Setting ($^\circ$)',fontsize=10)
    ax.set_zlabel('Differences $\Delta$ (%)',fontsize=10)
    ax.view_init(azim=-130, elev = 20)
    ax.set_zlim([4,8])


    plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)


##########################################################################################
# Plot 2 plot the number of point without Conforming (3D)
def plot_2_display():
    fig = plot.figure("Number of Points (No Conforming)",figsize=(10.5/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111,projection='3d')
    ax.plot_surface(Area_log, Angle, PointsNum, cmap='viridis', vmin= PointNum_min, vmax= PointNum_max)  

    ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
    ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

    ax.set_xlabel('Area Setting',fontsize=10)
    ax.set_ylabel('Angle Setting',fontsize=10)
    ax.set_zlabel('Number of Points (dB)',fontsize=10)

    ax.view_init(azim=50, elev = 20)
    plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)
##########################################################################################
# Plot 3 plot the triangle time without Conforming (3D)
def plot_3_display():
    fig = plot.figure("Triangle Time (No Conforming)",figsize=(10.5/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111,projection='3d')
    ax.plot_surface(Area_log, Angle, TriangleTime, cmap='viridis', vmin= TriangleTime_min, vmax= TriangleTime_max) 

    ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
    ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

    ax.set_xlabel('Area Setting',fontsize=10)
    ax.set_ylabel('Angle Setting',fontsize=10)
    ax.set_zlabel('Triangulation Time(dB$\mu s$)',fontsize=10)

    ax.view_init(azim=50, elev = 20)
    ax.set_zlim([0,900000])
    plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)


##########################################################################################
################             (With Conforming)
##########################################################################################
# Plot 4 plot the minimum distance with Conforming (3D)
def plot_4_display():
    fig = plot.figure("Minimum Distance (With Conforming)",figsize=(10.5/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111,projection='3d')
    ax.plot_surface(Area_log - np.log10(map_area), Angle, Result_smooth_conforming, cmap='viridis', vmin= result_min - 100, vmax= result_max - 100)

    # ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
    # ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

    ax.set_xlabel('Max area / Map area ($dB$)',fontsize=10)
    ax.set_ylabel('Angle Setting ($^\circ$)',fontsize=10)
    ax.set_zlabel('Differences $\Delta$ (%)',fontsize=10)
    ax.view_init(azim=-130, elev = 20)
    ax.set_zlim([4,8])
    plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)


##########################################################################################
# Plot 5 plot the number of point with Conforming (3D)
def plot_5_display():
    fig = plot.figure("Number of Points (With Conforming)",figsize=(10.5/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111,projection='3d')
    ax.plot_surface(Area_log, Angle, PointsNum_conforming, cmap='viridis', vmin= PointNum_min, vmax= PointNum_max) 

    ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
    ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

    ax.set_xlabel('Area Setting',fontsize=10)
    ax.set_ylabel('Angle Setting',fontsize=10)
    ax.set_zlabel('Number of Points (dB)',fontsize=10)

    ax.view_init(azim=50, elev = 20)
    plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)
##########################################################################################
# Plot 6 plot the triangle time with Conforming (3D)
def plot_6_display():
    fig = plot.figure("Triangle Time (With Conforming)",figsize=(10.5/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111,projection='3d')
    ax.plot_surface(Area_log, Angle, TriangleTime_conforming, cmap='viridis', vmin= TriangleTime_min, vmax= TriangleTime_max) 

    ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
    ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

    ax.set_xlabel('Area Setting',fontsize=10)
    ax.set_ylabel('Angle Setting',fontsize=10)
    ax.set_zlabel('Triangulation Time(dB$\mu s$)',fontsize=10)

    ax.view_init(azim=50, elev = 20)
    ax.set_zlim([0,900000])
    plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)


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

x_data = Area_log[1,:] - np.log10(map_area)
y1_data = Result[1,:]
y2_data = Result_conforming[1,:]

Area_log_Average, Result_average, Result_Std = findAverage(x_data, y1_data, 1e-1)
Area_log_Average_conforming, Result_average_conforming, Result_Std_conforming = findAverage(x_data, y2_data, 1e-1)


##### Display plot 7
def plot_7_display():
    fig = plot.figure("Mapping Area Vs Result",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)

    line1_handle = ax.scatter(x_data, y1_data,label="Without Conforming",s=1, color = "#6CB0D6")
    line2_handle, = ax.plot(Area_log_Average, Result_average, color="#0D4A70",label = "Without Conforming Average")
    line3_handle = ax.fill_between(Area_log_Average, np.array(Result_average) - Result_Std, np.array(Result_average) + Result_Std, color="#0D4A70", alpha=0.2, label='Standard Deviation')

    # line4_handle = ax.scatter(x_data, y2_data,label="With Conforming",s=1, color = "#CB181D")
    # line5_handle, = ax.plot(Area_log_Average_conforming, Result_average_conforming, color="#FB6A4A",label = "With Conforming Average")
    # line6_handle = ax.fill_between(Area_log_Average_conforming, np.array(Result_average_conforming) - Result_Std_conforming, np.array(Result_average_conforming) + Result_Std_conforming, color="#FCAE91", alpha=0.2, label='Standard Deviation')

    line4_handle = ax.scatter(x_data, y2_data,label="With Conforming",s=1, color = "#FD8D3C")
    line5_handle, = ax.plot(Area_log_Average_conforming, Result_average_conforming, color="#810026",label = "With Conforming Average")
    line6_handle = ax.fill_between(Area_log_Average_conforming, np.array(Result_average_conforming) - Result_Std_conforming, np.array(Result_average_conforming) + Result_Std_conforming, color="#FAB3A9", alpha=0.2, label='Standard Deviation')


    lines_handle = [line1_handle, line2_handle, line3_handle, line4_handle, line5_handle, line6_handle]
    lines_label  = ["Without Conforming Data", "Without Conforming Average", "Without Conforming Standard Divation","With Conforming Data", "With Conforming Average", "With Conforming Standard Divation"]

    # plot.title('Setted Maximum Area Vs Mapping Result',size=11)
    # plot.legend(ncol=3, loc = 'upper left')
    plot.legend(lines_handle, lines_label, ncol=2, loc = 'upper left')
    plot.xlabel('Max area / Map area ($dB$)',size=11)
    plot.ylabel('Path Length Differences $\Delta$ (%)',size=11)

    # ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    # ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    plot.grid()
    plot.tight_layout()

##### Plot 8 
def area_vs_point_fitting(x, a, b): # function for the fitting
    return a*x + b

angle_0_index = angle_data.index(0)
area_small_0_index = [i for i, v in enumerate(Area_log[angle_0_index,:]) if v < 0]
a,b = op.curve_fit(area_vs_point_fitting, Area_log[angle_0_index,area_small_0_index].T, PointsNum_log[angle_0_index,area_small_0_index].T)[0]
PointsNum_log_fit = [area_vs_point_fitting(x,a,b) for x in Area_log[angle_0_index,:].T]
PointsNum_log_fit_fun =  '$' +'{:.4}'.format(a)+"x " + '{:+.4}'.format(b)+ '$'
a,b = op.curve_fit(area_vs_point_fitting, Area_log[angle_0_index,area_small_0_index].T, PointsNum_log_conforming[angle_0_index,area_small_0_index].T)[0]
PointsNum_log_conforming_fit = [area_vs_point_fitting(x,a,b) for x in Area_log[angle_0_index,:].T]

def plot_8_display():
    fig = plot.figure("Mapping Area Vs Point Number",figsize=(21/2.54,9/2.54),dpi=200)

    ax = fig.add_subplot(111)
    ax.scatter(Area_log[angle_0_index,:], PointsNum_log[angle_0_index,:], label = "Without Conforming", color = "#6CB0D6", s = 2)
    ax.plot(Area_log[angle_0_index,:], PointsNum_log_fit, label = "Without Conforming Fitting", color = "#0D4A70")


    ax.scatter(Area_log[angle_0_index,:], PointsNum_log_conforming[angle_0_index,:], label = "With Conforming", color = "#FD8D3C", s = 2)
    ax.plot(Area_log[angle_0_index,:], PointsNum_log_conforming_fit, label = "With Conforming Fitting", color = "#810026")
    ax.legend()

    plot.xlabel('Max area settings',size=11)
    plot.ylabel('Points Number ($dBs$)',size=11)

    ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    # ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    plot.grid()
    plot.tight_layout()

def plot_8_1_display():
    fig = plot.figure("Mapping Area Vs Point Number",figsize=(9/2.54,7/2.54),dpi=200)

    ax = fig.add_subplot(111)
    ax.scatter(Area_log[angle_0_index,:]- np.log10(map_area), PointsNum_log[angle_0_index,:], label = "Without Conforming", color = "#6CB0D6", s = 2)
    # ax.plot(Area_log[angle_0_index,:], PointsNum_log_fit, label = "Without Conforming Fitting", color = "#0D4A70")
    ax.plot(Area_log[angle_0_index,:]- np.log10(map_area), PointsNum_log_fit, color = "#0D4A70")
    print(PointsNum_log_fit_fun)
    print(-0.99*np.log10(map_area))

    print(a)


    ax.scatter(Area_log[angle_0_index,:]- np.log10(map_area), PointsNum_log_conforming[angle_0_index,:], label = "With Conforming", color = "#FD8D3C", s = 2)
    # ax.plot(Area_log[angle_0_index,:], PointsNum_log_conforming_fit, label = "With Conforming Fitting", color = "#810026")
    ax.plot(Area_log[angle_0_index,:] - np.log10(map_area), PointsNum_log_conforming_fit, color = "#810026")
    ax.legend(prop={'size':9})

    ax.grid()

    plot.xlabel('Max area / Map area ($dB$)',size=11)
    plot.ylabel('Points Number ($dB$)',size=11)

    # ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
    # ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
    ax.set_xticks(([-5, -4,-3,-2])) 
    # ax.set_xticklabels(['$10^{-5}$', '$10^{-4}$', '$10^{-3}$', '$10^{-2}$']) 

    axins = inset_axes(ax, width="30%", height="30%",loc='lower left')
    axins.scatter(Area_log[angle_0_index,:]- np.log10(map_area), PointsNum_log[angle_0_index,:], label = "Zoom", color = "#6CB0D6", s = 5)
    axins.plot(Area_log[angle_0_index,:]- np.log10(map_area), PointsNum_log_fit, label = "Zoom", color = "#0D4A70")
    axins.scatter(Area_log[angle_0_index,:]- np.log10(map_area), PointsNum_log_conforming[angle_0_index,:], label = "Zoom", color = "#FD8D3C", s = 5)
    axins.plot(Area_log[angle_0_index,:]- np.log10(map_area), PointsNum_log_conforming_fit, label = "Zoom", color = "#810026")

    axins.set_xlim([-1.005 - np.log10(map_area),-0.995-np.log10(map_area)])
    axins.set_ylim([3.505,3.515])

    axins.xaxis.set_visible(False)
    axins.yaxis.set_visible(False)
    axins.grid()

    from mpl_toolkits.axes_grid1.inset_locator import mark_inset

    mark_inset(ax, axins, loc1=2, loc2=4, fc="none", ec= "0.5")





    plot.grid()
    plot.tight_layout()


##### Plot 9
#TODO: add the code for angle settings vs point number
def angle_vs_point_fitting(x, a, b): # function for the fitting
    return a*x + b


area_data_0_3 = area_data.index(0.0029084)

# area_data_0_3 = area_data.index(0.300466)
a,b = op.curve_fit(angle_vs_point_fitting, Angle[:,area_data_0_3].T, PointsNum_log[:,area_data_0_3].T)[0]
TriangleTime_log_fit_angle = [angle_vs_point_fitting(x,a,b) for x in Angle[:,area_data_0_3].T]
a,b = op.curve_fit(angle_vs_point_fitting, Angle[:,area_data_0_3].T, PointsNum_log_conforming[:,area_data_0_3].T)[0]
TriangleTime_log_conforming_fit_angle = [angle_vs_point_fitting(x,a,b) for x in Angle[:,area_data_0_3].T]

def plot_9_display():
    fig = plot.figure("Mapping Angle Vs Point Number",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    ax.scatter(Angle[:,area_data_0_3], PointsNum_log[:,area_data_0_3], label = "Without Conforming", color = "#6CB0D6", s = 10)
    # ax.plot(Angle[:,area_data_0_3], TriangleTime_log_fit_angle, label = "Without Conforming Fitting", color = "#0D4A70")

    ax.scatter(Angle[:,area_data_0_3], PointsNum_log[:,area_data_0_3], label = "With Conforming", color = "#FD8D3C", s = 3)
    # ax.plot(Angle[:,area_data_0_3], TriangleTime_log_conforming_fit_angle, label = "With Conforming Fitting", color = "#810026")
    ax.legend()

    plot.xlabel('Minimum Angle ($^\circ$)',size=11)
    plot.ylabel('Point Number (dB)',size=11)

    plot.grid()
    plot.tight_layout()


def plot_9_1_display():
    fig = plot.figure("Mapping Angle Vs Point Number",figsize=(9/2.54,7/2.54),dpi=200)
    ax = fig.add_subplot(111)
    ax.scatter(Angle[:,area_data_0_3], PointsNum_log[:,area_data_0_3], label = "Without Conforming", color = "#6CB0D6", s = 10)
    # ax.plot(Angle[:,area_data_0_3], TriangleTime_log_fit_angle, label = "Without Conforming Fitting", color = "#0D4A70")

    ax.scatter(Angle[:,area_data_0_3], PointsNum_log[:,area_data_0_3], label = "With Conforming", color = "#FD8D3C", s = 3)
    # ax.plot(Angle[:,area_data_0_3], TriangleTime_log_conforming_fit_angle, label = "With Conforming Fitting", color = "#810026")
    ax.legend(prop={'size':9})


    plot.xlabel('Minimum Angle ($^\circ$)',size=11)
    plot.ylabel('Point Number ($dB$)',size=11)

    # ax.set_xlim([-2.5,32.5])
    ax.set_ylim([5.0175,5.0425])
    ax.set_yticks(np.arange(5.02,5.04,0.005))

    plot.grid()
    plot.tight_layout()

def plot_9_2_display():
    area_data_10 = area_data.index(0.0029084)
    fig = plot.figure("Mapping Angle Vs Point Number (multiple area)",figsize=(9/2.54,7/2.54),dpi=200)
    ax = fig.add_subplot(111)
    ax.scatter(Angle[:,area_data_0_3], PointsNum_log[:,area_data_0_3], label = "Without Conforming", color = "#6CB0D6", s = 10)
    # ax.plot(Angle[:,area_data_0_3], TriangleTime_log_fit_angle, label = "Without Conforming Fitting", color = "#0D4A70")

    ax.scatter(Angle[:,area_data_0_3], PointsNum_log[:,area_data_0_3], label = "With Conforming", color = "#FD8D3C", s = 3)
    # ax.plot(Angle[:,area_data_0_3], TriangleTime_log_conforming_fit_angle, label = "With Conforming Fitting", color = "#810026")
    ax.legend(prop={'size':9})

    plot.xlabel('Mapping Angle Settings',size=11)
    plot.ylabel('Point Number ($dB$)',size=11)

    # ax.set_xlim([-2.5,32.5])
    ax.set_ylim([5.0175,5.0425])
    ax.set_yticks(np.arange(5.02,5.04,0.005))

    plot.grid()
    plot.tight_layout()


##### Plot 10
def area_vs_triangleTime_log_fitting(x, a, b): # function for the fitting
    return a*x + b

angle_0_index = angle_data.index(0)
angle_20_index = angle_data.index(20)
area_small_0_index = [i for i, v in enumerate(Area_log[angle_0_index,:]) if v < 0]
a,b = op.curve_fit(area_vs_triangleTime_log_fitting, Area_log[angle_0_index,area_small_0_index].T, TriangleTime_log[angle_0_index,area_small_0_index].T)[0]
TriangleTime_log_fit = [area_vs_triangleTime_log_fitting(x,a,b) for x in Area_log[angle_0_index,:].T]
a,b = op.curve_fit(area_vs_triangleTime_log_fitting, Area_log[angle_0_index,area_small_0_index].T, TriangleTime_log_conforming[angle_0_index,area_small_0_index].T)[0]
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
    ax.plot(PointsNum_log[angle_0_index,:], TriangleTime_log_fit_points_log, color = "#0D4A70")
    # ax.scatter(PointsNum_log[angle_20_index,:], TriangleTime_log[angle_20_index,:], s= 1, label = "Without Conforming", color = "#6CB0D6")#, s = 3)
 
    ax.scatter(PointsNum_log[angle_0_index,:], TriangleTime_log_conforming[angle_0_index,:],s= 1,  label = "With Conforming", color = "#FD8D3C")#, s = 3)
    ax.plot(PointsNum_log[angle_0_index,:], TriangleTime_log_conforming_fit_points_log, color = "#810026")
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



######## Plot 12
# Triangle Time vs Point number
def pointsNum_vs_triangleTime_fitting(x, a, b, c): # function for the fitting
    return a*x*np.log10(x) + b * x + c


# point_small_2_index = [i for i, v in enumerate(PointsNum_log[angle_0_index,:]) if v > 3]
a,b,c = op.curve_fit(pointsNum_vs_triangleTime_fitting, PointsNum[angle_0_index,point_small_2_index].T, TriangleTime[angle_0_index,point_small_2_index].T / 1e3)[0]
TriangleTime_fit_points = [pointsNum_vs_triangleTime_fitting(x,a,b,c) for x in PointsNum[angle_0_index,:].T]
TriangleTime_fit_points_fun =  '$' +'{:.4}'.format(a)+"x " + '{:+.4}'.format(b) + "x" + '{:+.4}'.format(c) + '$'
a,b,c = op.curve_fit(pointsNum_vs_triangleTime_fitting, PointsNum[angle_0_index,point_small_2_index].T, TriangleTime_conforming[angle_0_index,point_small_2_index].T / 1e3)[0]
TriangleTime_conforming_fit_points = [pointsNum_vs_triangleTime_fitting(x,a,b,c) for x in PointsNum[angle_0_index,:].T] 
TriangleTime_conforming_fit_points_fun =  '$' +'{:.4}'.format(a)+"x " + '{:+.4}'.format(b) + "x" + '{:+.4}'.format(c) + '$'


def plot_13_display():
    print(TriangleTime_fit_points_fun)
    print(TriangleTime_conforming_fit_points_fun)

    fig = plot.figure("Points Number Vs Triangle Time",figsize=(21/2.54,9/2.54),dpi=200)
    ax = fig.add_subplot(111)
    ax.scatter(PointsNum[angle_0_index,:], TriangleTime[angle_0_index,:]/1e3, s= 1, label = "Without Conforming", color = "#6CB0D6")#, s = 3)
    # ax.plot(PointsNum[angle_0_index,:], TriangleTime_fit_points, label = "Without Conforming Fitting", color = "#0D4A70")
    ax.plot(PointsNum[angle_0_index,:], TriangleTime_fit_points, color = "#0D4A70")
    # ax.scatter(PointsNum_log[angle_20_index,:], TriangleTime_log[angle_20_index,:], s= 1, label = "Without Conforming", color = "#6CB0D6")#, s = 3)
 
    ax.scatter(PointsNum[angle_0_index,:], TriangleTime_conforming[angle_0_index,:]/1e3,s= 1,  label = "With Conforming", color = "#FD8D3C")#, s = 3)
    # ax.plot(PointsNum[angle_0_index,:], TriangleTime_conforming_fit_points, label = "With Conforming Fitting", color = "#810026")
    ax.plot(PointsNum[angle_0_index,:], TriangleTime_conforming_fit_points, color = "#810026")
    # ax.scatter(PointsNum_log[angle_20_index,:], TriangleTime_log_conforming[angle_20_index,:],s= 1,  label = "With Conforming", color = "#FD8D3C")#, s = 3)
    
    ax.legend()

    plot.xlabel('Points number',size=11)
    plot.ylabel('Triangle Time ($ ms$)',size=11)

    plot.grid()
    plot.tight_layout()

if __name__ == "__main__":
    plot_1_display()
    # plot_2_display()
    # plot_3_display()
    plot_4_display()
    # plot_5_display()
    # plot_6_display()
    plot_7_display()
    # plot_8_display()
    # plot_9_display()
    # plot_10_display()
    # plot_11_display()

    # plots analysis the points number vs area | angle
    plot_8_1_display()
    plot_9_1_display()

    # plots analysis the triangle vs time
    # plot_12_1_display()
    plot_12_display()
    plot_13_display()

    plot.show()