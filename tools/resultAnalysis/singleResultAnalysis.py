import numpy as np
import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
from mpl_toolkits.mplot3d import Axes3D
from scipy.ndimage import gaussian_filter


################ Data reading ##############

# # read the first set of data
data_set = pd.read_csv("../../tests/squareMapTest/V3_3_1_Code_Test.csv")
# data_set = pd.read_csv("../../tests/RuggedMapTest/V6_4_1_Code_Test.csv")

minimum_distance = 20*np.sqrt(2)

# Datas
area_Set = data_set.loc[:,'Area Set'].values
angle_Set = data_set.loc[:,'Angle Set'].values
results = data_set.loc[:,'Result Distance'].values / minimum_distance * 100
points_num = data_set.loc[:,'Points Num'].values
triangle_time = data_set.loc[:,'Mapping Time(us)'].values
convertion_time = data_set.loc[:,'Convertion Time(us)'].values
pathplanning_time = data_set.loc[:,'Shorest Path Time(us)'].values
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
convertion_time_data = np.zeros([len(area_data), len(angle_data)])
convertion_time_data_conforming = np.zeros([len(area_data), len(angle_data)])
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
        convertion_time_data_conforming[x_loc, y_loc] = convertion_time[data_count]
        pathplanning_time_data_conforming[x_loc, y_loc] = pathplanning_time[data_count]
    else:
        results_data[x_loc, y_loc] = results[data_count]
        points_num_data[x_loc, y_loc] = points_num[data_count]
        triangle_time_data[x_loc, y_loc] = triangle_time[data_count]
        convertion_time_data[x_loc, y_loc] = convertion_time[data_count]
        pathplanning_time_data[x_loc, y_loc] = pathplanning_time[data_count]
        

total_time_data = triangle_time_data + convertion_time_data + pathplanning_time_data
total_time_data_conforming = triangle_time_data_conforming + convertion_time_data_conforming + pathplanning_time_data_conforming

###### Area and angle (x,y axis)
# Area, Angle = np.meshgrid(area_data, angle_data)
Area, Angle = np.meshgrid(area_data, angle_data)
Area_log = np.log10(Area)

print("********************************************")
print(Area.size)
print(triangle_time_data.size)
print("********************************************")


###### final result distance
# results_data = np.nan_to_num(results_data, nan=0)
# results_data_conforming = np.nan_to_num(results_data_conforming, nan=0)
Result = results_data.T
Result_conforming = results_data_conforming.T
Result_smooth = gaussian_filter(Result, sigma=3)  # Adjust sigma as needed
Result_smooth_conforming = gaussian_filter(Result_conforming, sigma=3)  # Adjust sigma as needed

# print(results_data)
result_min = min([min(min(row) for row in Result_smooth), min(min(row) for row in Result_smooth_conforming)])
result_max = max([max(max(row) for row in Result_smooth), max(max(row) for row in Result_smooth_conforming)])

# print(result_min)
# print(result_max)

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


###### Convertion Time
ConvertionTime = convertion_time_data.T
ConvertionTime_conforming = convertion_time_data_conforming.T
ConvertionTime_log = np.log10(ConvertionTime)
PathPlanningTime_log_conforming = np.log10(ConvertionTime_conforming)


PathPlanningTime_min = 0
PathPlanningTime_max = 750000

###### Triangle Time
PathPlanningTime = pathplanning_time_data.T
PathPlanningTime_conforming = pathplanning_time_data_conforming.T
PathPlanningTime_log = np.log10(PathPlanningTime)
PathPlanningTime_log_conforming = np.log10(PathPlanningTime_conforming)


PathPlanningTime_min = 0
PathPlanningTime_max = 750000


###### Total Time
TotalTime = total_time_data.T
TotalTime_conforming = total_time_data_conforming.T
TotalTime_log = np.log10(TotalTime)
TotalTime_log_conforming = np.log10(TotalTime_conforming)


TotalTime_min = 0
TotalTime_max = 750000

# Plot Setup
plot.rc('font',family='Arial')

##########################################################################################
def plot_1_display():
    fig = plot.figure("Cost time comparation in percentage",figsize=(21/2.54,9/2.54),dpi=200)
    

    setted_angle_index = list(Angle[:,1]).index(0)

    print("++++++++++++++++++++++++++++++++++++++++++=")
    print(Area[setted_angle_index,:])

    plot.fill_between(Area_log[setted_angle_index,:],  0,TriangleTime[setted_angle_index,:]/TotalTime[setted_angle_index,:] , color = '#009392', label='mapping time')
    plot.fill_between(Area_log[setted_angle_index,:], TriangleTime[setted_angle_index,:]/TotalTime[setted_angle_index,:] , (TriangleTime[setted_angle_index,:]+ConvertionTime[setted_angle_index,:])/TotalTime[setted_angle_index,:] , color='#39B185', label='convertion time')
    plot.fill_between(Area_log[setted_angle_index,:], (TriangleTime[setted_angle_index,:]+ConvertionTime[setted_angle_index,:])/TotalTime[setted_angle_index,:] , 1 , color='#E9E29C', label='shortest path time')


    plot.xticks(([-2.5, -2, -1.5, -1, -0.5, 0, 0.5]),['$10^{-2.5}$', '$10^{-2}$', '$10^{-1.5}$','$10^{-1}$', '$10^{-0.5}$', '$10^{ 0}$', '$10^{0.5}$'])

    plot.legend()
    plot.xlabel("Maximum Area Settings (dB)")
    plot.ylabel("Cost time ratio (%)")

    plot.tight_layout()





###########################################################################################
# Plot 2 Data processing


def pointNum_vs_ConvetionTime_log_fitting(x, a, b): # function for the fitting
    return a*x + b


setted_angle_index = list(Angle[:,1]).index(0)
a,b = op.curve_fit(pointNum_vs_ConvetionTime_log_fitting, PointsNum_log[setted_angle_index,:], ConvertionTime_log[setted_angle_index,:])[0]
ConvertionTime_log_fit = [pointNum_vs_ConvetionTime_log_fitting(x,a,b) for x in PointsNum_log[setted_angle_index,:]]

def plot_2_display():
    fig = plot.figure("Points Num vs Convertion time",figsize=(21/2.54,9/2.54),dpi=200)
    


    plot.scatter(PointsNum_log[setted_angle_index,:], ConvertionTime_log[setted_angle_index,:], label = "Test Result" , color = "#6CB0D6",s=5)
    plot.plot(PointsNum_log[setted_angle_index,:], ConvertionTime_log_fit, label = "Fit Result", color = "#0D4A70")
    plot.xlabel("Points Number (dB)",size=11)
    plot.ylabel("Convertion Time ($dB \mu s$)",size=11)
    plot.grid()
    plot.tight_layout()



plot_1_display()
plot_2_display()
plot.show()