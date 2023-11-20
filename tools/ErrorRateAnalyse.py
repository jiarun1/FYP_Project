import pandas as pd
import matplotlib.pyplot as plot
import numpy as np
import scipy.optimize as op 
import tkinter as tk
from tkinter.filedialog import askopenfilename




##############################################################3
# Data Reading

data = pd.read_csv("../tests/squareMapTest/log-2.csv")
data_col = data.columns.values
print(data_col)


area_set = data.loc[:,'Area Set']
line_num = data.loc[:,'Test Count']
mapping_time = data.loc[:,'Mapping Time(us)']
convertion_time = data.loc[:,'Convertion Time(us)']
shortestpath_time = data.loc[:,'Shorest Path Time(us)']
point_num = data.loc[:,'Points Num']
line_num = data.loc[:,'Path Num']
result_distance = data.loc[:,'Result Distance']

print(area_set)

total_time = mapping_time+convertion_time+shortestpath_time

shortestpath_time_max = max(shortestpath_time)
point_num_max = max(point_num)
total_time_max = max(total_time)
line_num_max = max(line_num)


##################################################################
# Data Processing
IdealMinDistance = 20* np.sqrt(2)
ErrorRate = (result_distance - IdealMinDistance) / IdealMinDistance

def point_vs_error(x, a, b): # function for the fitting
    return a*x + b

a,b = op.curve_fit(point_vs_error, point_num, ErrorRate)[0]
errorrate_fit_result = [point_vs_error(x,a,b) for x in point_num]
errorrate_fit_funtion = str((a))+"/x+" +str(b)
print("Mapping time information:", errorrate_fit_funtion)


####################################################################
# Plot overall settings
plot.rc('font',family='Arial')
plot.ion

# Plotting
plot.figure("Mapping Points Vs Error",figsize=(21/2.54,9/2.54),dpi=200)
plot.scatter(point_num, ErrorRate,label="Test Result",s=10)
# plot.plot(point_num, errorrate_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Number of Points Vs Mapping Time',size=11)
plot.legend()
plot.xlabel('Number of Points',size=11)
plot.ylabel('Accuracy',size=11)
plot.grid()


plot.tight_layout()
plot.show()

wait = input("Press Enter to continue.")