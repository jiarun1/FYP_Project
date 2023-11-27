import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
import tkinter as tk
from tkinter.filedialog import askopenfilename

##############################################################3
# Data Processing

# data = pd.read_csv("../tests/squareMapTest/log-2.csv")
data = pd.read_csv("../tests/squareMapTest/V1_1_Code_Test.csv")
data_col = data.columns.values
print(data_col)


area_set = data.loc[:,'Area Set'].values
line_num = data.loc[:,'Test Count'].values
mapping_time = data.loc[:,'Mapping Time(us)'].values
convertion_time = data.loc[:,'Convertion Time(us)'].values
shortestpath_time = data.loc[:,'Shorest Path Time(us)'].values
point_num = data.loc[:,'Points Num'].values
line_num = data.loc[:,'Path Num'].values
result_dif = data.loc[:,'Result Distance'].values

print(area_set)

total_time = mapping_time+convertion_time+shortestpath_time

shortestpath_time_max = max(shortestpath_time)
point_num_max = max(point_num)
total_time_max = max(total_time)
line_num_max = max(line_num)



##################################################################33
########## Data Analysing ##############
# Curve Fitting

#-------------------------------------------------------------
# mapping area vs time fitting
def area_vs_time_fitting(x, a, b): # function for the fitting
    return a/x + b

a,b = op.curve_fit(area_vs_time_fitting, area_set, mapping_time)[0]
mappingtime_fit_result = [area_vs_time_fitting(x,a,b) for x in area_set]
mappingtime_fit_funtion = str((a))+"/x+" +str(b)
print("Mapping time information:", mappingtime_fit_funtion)

#-------------------------------------------------------------
# mapping area vs point fitting
def area_vs_point_fitting(x, a, b): # function for the fitting
    return a/x + b

a,b = op.curve_fit(area_vs_point_fitting, area_set, point_num)[0]
mapping_point_fit_result = [area_vs_point_fitting(x,a,b) for x in area_set]
mapping_point_fit_funtion = str((a))+"/x+" +str(b)
print("Mapping area vs point information:", mapping_point_fit_funtion)

#-------------------------------------------------------------
# mapping points vs time fitting
def mapping_point_vs_time_fitting(x, a, b): # function for the fitting
    return a*x + b

a,b = op.curve_fit(mapping_point_vs_time_fitting, point_num, mapping_time)[0]
mapping_point_vs_time_fit_result = [mapping_point_vs_time_fitting(x,a,b) for x in point_num]
mapping_point_vs_time_fit_funtion = str((a))+"/x+" +str(b)
print("Mapping Points vs time information:", mapping_point_vs_time_fit_funtion)


#-------------------------------------------------------------
# convertion curve fitting
def convertion_fitting(x, a, b): # function for the fitting
    return a*x*x+b

a,b = op.curve_fit(convertion_fitting, point_num, convertion_time)[0]
convertion_fit_result = [convertion_fitting(x,a,b) for x in point_num]
convertion_fit_function = str((a))+"x^2+" +str(b)
print("Convertion Fixed Result:", convertion_fit_function)

#-------------------------------------------------------------
# dijkstra curve fitting
def dijkstra_fitting(x, a, b, c): # function for the fitting
    return a*x*x + b*np.log(x)*x + c

a,b,c = op.curve_fit(dijkstra_fitting, point_num, shortestpath_time)[0]
shortestpath_fit_result = [dijkstra_fitting(x,a,b,c) for x in point_num]
shortestpath_fit_function = str((a))+"x^2+" +str(b) + "x+" + str(c)
print("Shortest Path Fixed Result:", shortestpath_fit_function)




##################################################################33
# Plot overall settings
plot.rc('font',family='Arial')
plot.ion()
########## Plot drawing 1 ##############
# general setting

# plot 1, mapping time
plot.figure("Mapping Plot",figsize=(21/2.54,9/2.54),dpi=200)
plot.scatter(area_set,mapping_time,label="Test Result",s=10)
plot.plot(area_set, mappingtime_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Setted Maximum Area Vs Mapping Time',size=11)
plot.legend()
plot.xlabel('Max area settings',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()
plot.tight_layout()
plot.xlim([0,1.5])

# plot 2, mapping time
plot.figure("Mapping Area Vs Points",figsize=(21/2.54,9/2.54),dpi=200)
plot.scatter(area_set,point_num,label="Test Result",s=10)
plot.plot(area_set, mapping_point_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Setted Maximum Area Vs Mapping Points',size=11)
plot.legend()
plot.xlabel('Max area settings',size=11)
plot.ylabel('Points Num',size=11)
plot.grid()
plot.tight_layout()

# plot 3
plot.figure("Mapping Points Vs Time",figsize=(21/2.54,9/2.54),dpi=200)
plot.scatter(point_num, mapping_time,label="Test Result",s=10)
plot.plot(point_num, mapping_point_vs_time_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Mapping points Vs Mapping Time',size=11)
plot.legend()
plot.xlabel('Mapping Points Result',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()
plot.tight_layout()

# plot 4
plot.figure("Convertion Plot",figsize=(21/2.54,9/2.54),dpi=200)
plot.scatter(point_num, convertion_time ,label="Test Result")
plot.plot(point_num, convertion_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Point Number Vs Convertion Execution Time',size=11)
plot.legend()
plot.xlabel('Point Number',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()

# # plot 5
plot.figure("Shortest Path Plot",figsize=(21/2.54,9/2.54),dpi=200)
plot.scatter(point_num, shortestpath_time ,label="Test Result")
plot.plot(point_num, shortestpath_fit_result, color="red", label = "Curve Fitted Result")
plot.title('Point Number Vs Shortest Path Execution Time',size=11)
plot.legend()
plot.xlabel('Point Number',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()

# # plot 6 Time cost percentage for the system
plot.figure("Time Cost Percentage with point number",figsize=(21/2.54,9/2.54),dpi=200)
plot.fill_between(point_num, mapping_time/total_time , color = '#009392', label='mapping time')
plot.fill_between(point_num, mapping_time/total_time , (mapping_time+convertion_time)/total_time , color='#39B185', label='convertion time')
plot.fill_between(point_num, (mapping_time+convertion_time)/total_time, 1 , color='#E9E29C', label='shortest path time')
plot.title('Time cost for each function',size=11)
plot.legend()
plot.xlabel('Point Number',size=11)
plot.ylabel(r'Cost time in percentage (%)',size=11)
plot.grid()
plot.xlim([0,1000])


plot.tight_layout()
plot.show()

wait = input("Press Enter to continue.")