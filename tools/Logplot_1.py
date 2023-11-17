import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
import tkinter as tk
from tkinter.filedialog import askopenfilename




##############################################################3
# Data Processing

data = pd.read_csv("../tests/performance_test_1/log-2.csv")
data_col = data.columns.values
print(data_col)


area_set = data.loc[:,'Area Set']
line_num = data.loc[:,'Test Count']
mapping_time = data.loc[:,'Mapping Time(us)']
convertion_time = data.loc[:,'Convertion Time(us)']
shortestpath_time = data.loc[:,'Shorest Path Time(us)']
point_num = data.loc[:,'Points Num']
line_num = data.loc[:,'Path Num']
result_dif = data.loc[:,'Result Distance']

print(area_set)

total_time = mapping_time+convertion_time+shortestpath_time

shortestpath_time_max = max(shortestpath_time)
point_num_max = max(point_num)
total_time_max = max(total_time)
line_num_max = max(line_num)



##################################################################33
########## Data Analysing ##############
# Curve Fitting
def dijkstra_fitting(x, a, b,c):
    return a*x*x+b*x+c

def triangle_fitting(x, a, b, c):
    return a*x*x + b*x+ c

# dijkstra curve fitting
a,b,c = op.curve_fit(dijkstra_fitting, point_num, shortestpath_time)[0]
shortestpath_fit_result = [dijkstra_fitting(x,a,b,c) for x in point_num]
shortestpath_fit_function = str((a))+"x^2+" +str(b)+"x+" + str(c)
print("Point Fixed Result:", shortestpath_fit_function)


# mapping curve fitting
a,b,c = op.curve_fit(triangle_fitting, area_set, mapping_time)[0]
mappingtime_fit_result = [triangle_fitting(x,a,b,c) for x in area_set]
mappingtime_fit_funtion = str((a))+"$x^2$+" +str(b) + "$x$"+str(c)
print("Mapping time information:", mappingtime_fit_funtion)






##################################################################33
# Plot overall settings
plot.rc('font',family='Arial')
plot.ion
########## Plot drawing 1 ##############
# general setting

# plot 1, mapping time
# plot.subplot(2,1,1)
plot.figure("Mapping Plot",figsize=(21/2.54,9/2.54),dpi=200)
plot.semilogx(area_set,mapping_time,label="Test Result")
plot.plot(area_set, mappingtime_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Setted Maximum Area Vs Mapping Time',size=11)
plot.legend()
plot.xlabel('Max area settings',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()
plot.tight_layout()


# plot 2
# plot.subplot(2,1,2)
plot.figure("Shortest Path Plot",figsize=(21/2.54,9/2.54),dpi=200)
plot.scatter(point_num, shortestpath_time ,label="Test Result")
plot.plot(point_num, shortestpath_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Point Number Vs Shortest Path Execution Time',size=11)
plot.legend()
plot.xlabel('Point Number',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()

plot.tight_layout()
plot.show()

wait = input("Press Enter to continue.")