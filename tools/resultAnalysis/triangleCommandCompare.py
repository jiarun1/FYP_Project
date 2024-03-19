import numpy as np
import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
from mpl_toolkits.mplot3d import Axes3D
from scipy.ndimage import gaussian_filter


################ Data reading ##############

# read the first set of data
data_set = pd.read_csv("../../tests/squareMapTest/V3_3_Code_Test.csv")

minimum_distance = 20*np.sqrt(2)

# Datas
area_Set = data_set.loc[:,'Area Set'].values
angle_Set = data_set.loc[:,'Angle Set'].values
results = data_set.loc[:,'Result Distance'].values / minimum_distance * 100
points_num = data_set.loc[:,'Points Num'].values
triangle_time = data_set.loc[:,'Mapping Time(us)'].values
is_conforming = data_set.loc[:,'If Conforming'].values



area_data = sorted(list(set(area_Set)))
area_data = [x for x in area_data if x < 5]
angle_data = sorted(list(set(angle_Set)))
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
TriangleTime_log = np.log(TriangleTime)
TriangleTime_log_conforming = np.log(TriangleTime_conforming)


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

fig = plot.figure("Minimum Distance (No Conforming)",figsize=(10.5/2.54,9/2.54),dpi=200)
ax = fig.add_subplot(111,projection='3d')
ax.plot_surface(Area_log, Angle, Result_smooth, cmap='viridis', vmin= result_min, vmax= result_max) 

ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

ax.set_xlabel('Area Setting ',fontsize=10)
ax.set_ylabel('Angle Setting',fontsize=10)
ax.set_zlabel('Minimum Distance (%)',fontsize=10)
ax.view_init(azim=-130, elev = 20)
ax.set_zlim([104,108])


plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)


##########################################################################################
# Plot 2 plot the number of point without Conforming (3D)
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

fig = plot.figure("Minimum Distance (With Conforming)",figsize=(10.5/2.54,9/2.54),dpi=200)
ax = fig.add_subplot(111,projection='3d')
ax.plot_surface(Area_log, Angle, Result_smooth_conforming, cmap='viridis', vmin= result_min, vmax= result_max)

ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

ax.set_xlabel('Area Setting',fontsize=10)
ax.set_ylabel('Angle Setting',fontsize=10)
ax.set_zlabel('Minimum Distance (%)',fontsize=10)
ax.view_init(azim=-130, elev = 20)
ax.set_zlim([104,108])
plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)


##########################################################################################
# Plot 5 plot the number of point with Conforming (3D)
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

x_data = Area_log[1,:]
y1_data = Result[1,:]
y2_data = Result_conforming[1,:]

fig = plot.figure("Mapping Area Vs Result",figsize=(21/2.54,9/2.54),dpi=200)
ax = fig.add_subplot(111)
Area_log_Average, Result_average, Result_Std = findAverage(x_data, y1_data, 1e-1)
Area_log_Average_conforming, Result_average_conforming, Result_Std_conforming = findAverage(x_data, y2_data, 1e-1)



line1_handle = ax.scatter(x_data, y1_data,label="Without Conforming",s=1, color = "#6CB0D6")
line2_handle, = ax.plot(Area_log_Average, Result_average, color="#0D4A70",label = "Without Conforming Average")
line3_handle = ax.fill_between(Area_log_Average, np.array(Result_average) - Result_Std, np.array(Result_average) + Result_Std, color="#0D4A70", alpha=0.2, label='Standard Deviation')


line4_handle = ax.scatter(x_data, y2_data,label="With Conforming",s=1, color = "#FD8D3C")
line5_handle, = ax.plot(Area_log_Average_conforming, Result_average_conforming, color="#810026",label = "With Conforming Average")
line6_handle = ax.fill_between(Area_log_Average_conforming, np.array(Result_average_conforming) - Result_Std_conforming, np.array(Result_average_conforming) + Result_Std_conforming, color="#FAB3A9", alpha=0.2, label='Standard Deviation')


lines_handle = [line1_handle, line2_handle, line3_handle, line4_handle, line5_handle, line6_handle]
lines_label  = ["No Conforming Data", "No Conforming Average", "No Conforming Standard Divation","Conforming Data", "Conforming Average", "Conforming Standard Divation"]

plot.title('Setted Maximum Area Vs Mapping Result',size=11)
# plot.legend(ncol=3, loc = 'upper left')
plot.legend(lines_handle, lines_label, ncol=2, loc = 'upper left')
plot.xlabel('Max area settings',size=11)
plot.ylabel('Minimum Distance (%)',size=11)

ax.set_xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5])) 
ax.set_xticklabels(['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$']) 
plot.grid()
plot.tight_layout()

##### Plot 8 
#TODO: add the code for area vs point number
def area_vs_point_fitting(x, a, b): # function for the fitting
    return a/x + b

a,b = op.curve_fit(area_vs_point_fitting, Area_log[1,:].T, PointsNum_log[1,:].T)[0]
area_pointNum_fit = [area_vs_point_fitting(x,a,b) for x in Area_log[1,:].T]
area_pointNum_fit_function = str((a))+"/x+" +str(b)
print("Mapping area vs point information:", area_pointNum_fit_function)


fig = plot.figure("Mapping Area Vs Point Number",figsize=(21/2.54,9/2.54),dpi=200)





##### Plot 9
#TODO: add the code for angle settings vs point number
def area_vs_point_fitting(x, a, b): # function for the fitting
    return a/x + b

a,b = op.curve_fit(area_vs_point_fitting, Area_log[1,:].T, PointsNum_log[1,:].T)[0]
area_pointNum_fit = [area_vs_point_fitting(x,a,b) for x in Area_log[1,:].T]
area_pointNum_fit_function = str((a))+"/x+" +str(b)
print("Mapping area vs point information:", area_pointNum_fit_function)


##### Plot 10
#TODO: add the code for area vs triangle time
def area_vs_point_fitting(x, a, b): # function for the fitting
    return a/x + b

a,b = op.curve_fit(area_vs_point_fitting, Area_log[1,:].T, PointsNum_log[1,:].T)[0]
area_pointNum_fit = [area_vs_point_fitting(x,a,b) for x in Area_log[1,:].T]
area_pointNum_fit_function = str((a))+"/x+" +str(b)
print("Mapping area vs point information:", area_pointNum_fit_function)


fig = plot.figure("Mapping Area Vs Point Number",figsize=(21/2.54,9/2.54),dpi=200)





##### Plot 11
#TODO: add the code for angle settings vs triangle time
def area_vs_point_fitting(x, a, b): # function for the fitting
    return a/x + b

a,b = op.curve_fit(area_vs_point_fitting, Area_log[1,:].T, PointsNum_log[1,:].T)[0]
area_pointNum_fit = [area_vs_point_fitting(x,a,b) for x in Area_log[1,:].T]
area_pointNum_fit_function = str((a))+"/x+" +str(b)
print("Mapping area vs point information:", area_pointNum_fit_function)





plot.show()