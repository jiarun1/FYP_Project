import matplotlib.pyplot as plot
import numpy as np

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


fig = plot.figure()
ax1 = fig.add_subplot(211)
ax2 = fig.add_subplot(212)

ax1.scatter(point_num,cost_time)
ax2.scatter(line_num,cost_time)

##################################################################33
########## Plot fitting ##############
a,b,c = np.polyfit(point_num,cost_time,2)
point_fit_result = [a*np.square(x)+b*x+c for x in point_num]
ax1.plot(point_num, point_fit_result, color="red")

a,b,c = np.polyfit(line_num,cost_time,2)
line_fit_result = [a*np.square(x)+b*x+c for x in line_num]
ax2.plot(line_num, line_fit_result, color="red")


plot.show()

fo.close()

wait = input("Press Enter to continue.")