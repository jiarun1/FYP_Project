import numpy as np
import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
from mpl_toolkits.mplot3d import Axes3D
from scipy.ndimage import gaussian_filter


################ Data reading ##############

# read the first set of data
data_set = pd.read_csv("../../tests/squareMapTest/V3_2_Code_Test.csv")

data_set_version_string = "Triditional"

# Datas
area_Set = data_set.loc[:,'Area Set'].values
angle_Set = data_set.loc[:,'Angle Set'].values
results = data_set.loc[:,'Result Distance'].values
points_num = data_set.loc[:,'Points Num'].values
triangle_time = data_set.loc[:,'Mapping Time(us)'].values

area_data = sorted(list(set(area_Set)))
area_data = [x for x in area_data if x < 5]
angle_data = sorted(list(set(angle_Set)))
results_data = np.zeros([len(area_data), len(angle_data)])
points_num_data = np.zeros([len(area_data), len(angle_data)])
triangle_time_data = np.zeros([len(area_data), len(angle_data)])
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
    
    results_data[x_loc, y_loc] = results[data_count]
    points_num_data[x_loc, y_loc] = points_num[data_count]
    triangle_time_data[x_loc, y_loc] = triangle_time[data_count]
    

results_data = np.nan_to_num(results_data, nan=0)
Area, Angle = np.meshgrid(area_data, angle_data)
Result = results_data.T
PointsNum = points_num_data.T
TriangleTime = triangle_time_data.T
Result_smooth = gaussian_filter(Result, sigma=2)  # Adjust sigma as needed
Area_log = np.log10(Area)
PointsNum_log = np.log10(PointsNum)
TriangleTime_log = np.log(TriangleTime)
print(results_data)

print("xdata_size", Area.shape[0], Area.shape[1])
print("ydata_size", Angle.shape[0], Angle.shape[1])
print("zdata_size", results_data.shape[0], results_data.shape[1])

#########################
## Plot:

# Plot Setup
plot.rc('font',family='Arial')
#plot.ion()
fig = plot.figure("Minimum Distance",figsize=(10.5/2.54,9/2.54),dpi=200)
ax = fig.add_subplot(111,projection='3d')
# ax.scatter(X, Y, z_data, s = 3)#, cmap='viridis')  
ax.plot_surface(Area_log, Angle, Result_smooth, cmap='viridis') 

ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

ax.set_xlabel('Area Setting',fontsize=10)
ax.set_ylabel('Angle Setting',fontsize=10)
ax.set_zlabel('Minimum Distance',fontsize=10)
ax.view_init(azim=-130, elev = 20)
plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)
# plot.tight_layout()
##########################################################################################
fig = plot.figure("Number of Points",figsize=(10.5/2.54,9/2.54),dpi=200)
ax = fig.add_subplot(111,projection='3d')
# ax.scatter(X, Y, z_data, s = 3)#, cmap='viridis')  
ax.plot_surface(Area_log, Angle, PointsNum, cmap='viridis') 

ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

ax.set_xlabel('Area Setting',fontsize=10)
ax.set_ylabel('Angle Setting',fontsize=10)
ax.set_zlabel('Number of Points (dB)',fontsize=10)

ax.view_init(azim=50, elev = 20)
plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)
######################################################################################
fig = plot.figure("Triangle Time",figsize=(10.5/2.54,9/2.54),dpi=200)
ax = fig.add_subplot(111,projection='3d')
# ax.scatter(X, Y, z_data, s = 3)#, cmap='viridis')  
ax.plot_surface(Area_log, Angle, TriangleTime, cmap='viridis') 

ax.set_xticks(np.log10([1e1, 1e0, 1e-1, 1e-2,1e-3])) 
ax.set_xticklabels(['$10^1$', '$10^0$', '$10^{-1}$','$10^{-2}$', '$10^{-3}$']) 

ax.set_xlabel('Area Setting',fontsize=10)
ax.set_ylabel('Angle Setting',fontsize=10)
ax.set_zlabel('Triangulation Time(dB$\mu s$)',fontsize=10)

ax.view_init(azim=50, elev = 20)
plot.subplots_adjust(top=0.9, bottom=0.1, left=0.1, right=1, hspace=0.4, wspace=0.4)
#########################################################################################
# plot 2, mapping area vs point
print(Area_log[1,:])
# mapping area vs point fitting
def area_vs_point_fitting(x, a, b): # function for the fitting
    return a/x + b

a,b = op.curve_fit(area_vs_point_fitting, Area_log[1,:].T, PointsNum_log[1,:].T)[0]
area_pointNum_fit = [area_vs_point_fitting(x,a,b) for x in Area_log[1,:]]
area_pointNum_fit_function = str((a))+"/x+" +str(b)
print("Mapping area vs point information:", area_pointNum_fit_function)


plot.figure("Mapping Area Vs Points",figsize=(21/2.54,9/2.54),dpi=200)
plot.scatter(Area_log[1,:], PointsNum_log[1,:],label="Test Result",s=10)
plot.plot(Area_log[1,:], area_pointNum_fit, color="red",label = "Curve Fitted Result")
plot.title('Setted Maximum Area Vs Mapping Points',size=11)
plot.legend()
plot.xlabel('Max area settings',size=11)
plot.ylabel('Points Num',size=11)
plot.grid()
plot.tight_layout()


plot.show()