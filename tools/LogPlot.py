import matplotlib.pyplot as plot
import numpy as np

# import tkinter as tk

# from tkinter.filedialog import  askopenfilename

# tk.Tk().withdraw()

# filename = askopenfilename()
# print(filename)

fo = open("../tests/performance_test_1/log-1.txt","r")

count = 0

point_num = []
cost_time = []
line_num = []
while 1:
    line = fo.readline()
    count = count + 1
    if line is None:
        break
    if count == 1:
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
plot.subplot(2,1,1)
plot.scatter(point_num,cost_time)

# fit curve
a,b,c = np.polyfit(point_num,cost_time,2)
point_fit_result = [a*np.square(x)+b*x+c for x in point_num]
plot.plot(point_num, point_fit_result, color="red")
plot.title('Point Number Vs Execution Time')
plot.legend(["Test Result", "Curve Fitted Result"])
plot.xlabel('Point Number')
plot.ylabel('Cost time ($ \mu s $)')
plot.grid()

plot.subplot(2,1,2)
plot.scatter(line_num,cost_time)
a,b,c = np.polyfit(line_num,cost_time,2)
line_fit_result = [a*np.square(x)+b*x+c for x in line_num]
plot.plot(line_num, line_fit_result, color="red")
plot.title('Line Number Vs Execution Time')
plot.legend(["Test Result", "Curve Fitted Result"])
plot.xlabel('Line Number')
plot.ylabel('Cost time ($ \mu s $)')
plot.grid()

plot.tight_layout()
plot.show()


fo.close()

wait = input("Press Enter to continue.")