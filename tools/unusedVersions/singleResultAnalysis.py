import numpy as np
import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
from mpl_toolkits.mplot3d import Axes3D
from scipy.ndimage import gaussian_filter


################ Data reading ##############

# read the first set of data
data = pd.read_csv("../../tests/squareMapTest/V2_Code_Test.csv")

minimum_distance = 20*np.sqrt(2)

# Datas
Area = data.loc[:,'Area Set'].values
line_num = data.loc[:,'Test Count'].values
triangle_time_data = data.loc[:,'Mapping Time(us)'].values
convertion_time_data = data.loc[:,'Convertion Time(us)'].values
pathplanning_time_data = data.loc[:,'Shorest Path Time(us)'].values
points_num_data = data.loc[:,'Points Num'].values
line_num = data.loc[:,'Path Num'].values
results_data = (data.loc[:,'Result Distance'].values - minimum_distance) / minimum_distance
   

total_time_data = triangle_time_data + convertion_time_data + pathplanning_time_data


###### final result distance
# results_data = np.nan_to_num(results_data, nan=0)
# results_data_conforming = np.nan_to_num(results_data_conforming, nan=0)
Result = results_data
Result_smooth = gaussian_filter(Result, sigma=3)  # Adjust sigma as needed

# print(results_data)
result_min = min(Result)
result_max = max(Result)

# print(result_min)
# print(result_max)

result_min = 104
result_max = 107.5



###### Point Num Generated
PointsNum = points_num_data
PointsNum_log = np.log10(PointsNum)


PointNum_min = 0
PointNum_max = 175000

###### Triangle Time
TriangleTime = triangle_time_data.T
TriangleTime_log = np.log10(TriangleTime)


TriangleTime_min = 0
TriangleTime_max = 750000


###### Convertion Time
ConvertionTime = convertion_time_data.T
ConvertionTime_log = np.log10(ConvertionTime)


PathPlanningTime_min = 0
PathPlanningTime_max = 750000

###### Triangle Time
PathPlanningTime = pathplanning_time_data.T
PathPlanningTime_log = np.log10(PathPlanningTime)


PathPlanningTime_min = 0
PathPlanningTime_max = 750000


###### Total Time
TotalTime = total_time_data.T
TotalTime_log = np.log10(TotalTime)

TotalTime_min = 0
TotalTime_max = 750000

# Plot Setup
plot.rc('font',family='Arial')




def plot_3_display():

    setted_area_index = list(Area).index(0.0100885)

    # the pie plot 
    fig = plot.figure("Time cost compare",figsize=(9/2.54,9/2.54),dpi=200)

    label = ["Triangulation","Dijkstra's","Conversion"]
    data = [TriangleTime[setted_area_index],
            PathPlanningTime[setted_area_index],
            ConvertionTime[setted_area_index]
    ]
    colors = ["#87CEEB","#FF7F50","#2E8B57"]

    plot.pie(data, labels= label,autopct='%1.1f%%',colors=colors)

    plot.tight_layout()



plot_3_display()
plot.show()