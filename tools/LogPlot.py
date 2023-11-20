import matplotlib.pyplot as plot
import numpy as np

# import tkinter as tk

# from tkinter.filedialog import  askopenfilename

# tk.Tk().withdraw()

# filename = askopenfilename()
# print(filename)

fo = open("../tests/squareMapTest/log-2.txt","r")

count = 0

point_num = []
cost_time = []
line_num = []
while 1:
    line = fo.readline()
    count = count + 1
    if line is None:
        break
    if count <= 100:
        continue
    if count >= 2000:
        break


    line_split = line.split(" ")
    # print(line_split)

    point_num.append(int(line_split[2]))
    cost_time.append(int(line_split[1]))
    line_num.append(int(line_split[3]))

    # print(point_num)


point_num_max = max(point_num)
cost_time_max = max(cost_time)
line_num_max = max(line_num)



##################################################################33
########## Plot drawing 1 ##############
plot.figure(figsize=(21/2.54,15/2.54))
plot.rc('font',family='Arial')

plot.subplot(2,1,1)
plot.scatter(point_num,cost_time,label="Test Result")
# fit curve
a,b,c = np.polyfit(point_num,cost_time,2)
point_fit_result = [a*np.square(x)+b*x+c for x in point_num]
fixed_function = str((a))+"$x^2$+" + str((b)) + "x+"+str((c))
print(fixed_function)
plot.plot(point_num, point_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Point Number Vs Execution Time',size=11)
# plot.legend(["Test Result","Curve Fitted Result"])
plot.legend()
plot.xlabel('Point Number',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()

plot.subplot(2,1,2)
plot.scatter(line_num,cost_time,label="Test Result")
a,b,c = np.polyfit(line_num,cost_time,2)
line_fit_result = [a*np.square(x)+b*x+c for x in line_num]
fixed_function = str((a))+"$x^2$+" + str((b)) + "x+"+str((c))
print(fixed_function)
plot.plot(line_num, line_fit_result, color="red",label = "Curve Fitted Result")
plot.title('Line Number Vs Execution Time',size=11)
plot.legend()
plot.xlabel('Point Number',size=11)
plot.ylabel('Cost time ($ \mu s $)',size=11)
plot.grid()


plot.tight_layout()
plot.show()


fo.close()

wait = input("Press Enter to continue.")