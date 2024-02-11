import matplotlib.pyplot as plt
import tkinter as tk
import sys
import getopt
from tkinter.filedialog import  askopenfilename
from enum import Enum

class verticleParam_e(Enum):
    NUMBER = 0
    COORDINATE = int(1)
    PROPORITIES = int(2)
    IS_BOUNDARY = int(3)

class verticleProporityParam_e(Enum):
    IS_PATH = int(0)
    POINT_TYPE = int(1)

class pointColour(Enum):
    NORMAL_POINT = "#A0A0A0"
    SHORTEST_POINT = '#FFA500'
    MIDDLE_POINT = '#6495ED'

class lineColour(Enum):
    NORMAL_Line = "#696969"
    SHORTEST_Line = '#FFA500'
    MIDDLE_Line = '#6495ED'

#polyFilePath = "/home/jiarun/FYP_Project/code/tests/squareMapTest/Result.poly"
defaultFilePath = "/home/jiarun/Desktop/FYP/code/tests/squareMapTest/Result.poly"


def read_poly_file(file_path):
    vertices = []
    polygons = []

    with open(file_path, 'r') as file:

        # Reading vertices
        num_vertices, num_dimension, num_property, num_boundary  = map(int, file.readline().split())
        for _ in range(num_vertices):
            line = file.readline().strip()

            # ignore the comment line
            if(line.find("#") != -1):
                continue

            parts = line.split()
            # if(parts.count() < (1 + num_dimension + num_property + num_boundary)):
            #     print("Reading verticles failed")
            #     exit()

            item_count = 0
            coordinates = list()
            propertys = list()

            vertex_index = int(parts[item_count])
            for _ in range(num_dimension):
                item_count += 1
                coordinates.append(float(parts[item_count]))

            for _ in range(num_property):
                item_count += 1
                propertys.append(int(parts[item_count]))
                
            vertices.append((vertex_index, coordinates, propertys))

        # Reading polygons
        num_polygons, num_boundary  = map(int, file.readline().split())
        for _ in range(num_polygons):
            line = file.readline().strip()

            # ignore the comment line
            if(line.find("#") != -1):
                continue

            parts = line.split()
            polygon_index = int(parts[0])
            vertex_indices = list([int(parts[1]), int(parts[2])])
            polygons.append((polygon_index, vertex_indices))

    return vertices, polygons

def main(polyPath):

    print(polyPath)
    # tk.Tk().withdraw()
    # polyFilePath = askopenfilename()
    vertices, polygons = read_poly_file(polyPath)
    # print(vertices)
    # print(vertices[1][1][1])
    # print(len(vertices))

    plt.rc('font',family='Arial')
    #plt.ion()
    plt.figure()

    for i in range(len(vertices)):
        # print(vertices[i])
        if(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 1):
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.MIDDLE_POINT.value, markersize='3')
        # elif(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.IS_PATH.value] == 1):
        #     plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointAndConnectionColour.SHORTEST_POINT.value, markersize='3')
        else:
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.NORMAL_POINT.value, markersize='3')



    for i in range(len(polygons)):
        # print(polygons[i])

        # this 2 value is the overall point detail list
        point1 = vertices[polygons[i][1][0] - 1]
        point2 = vertices[polygons[i][1][1] - 1]

        # this 2 value is the x value list of the 2 points and the y value list of the points
        point_x = [point1[verticleParam_e.COORDINATE.value][0], point2[verticleParam_e.COORDINATE.value][0]]
        point_y = [point1[verticleParam_e.COORDINATE.value][1], point2[verticleParam_e.COORDINATE.value][1]]
        
        if (point1[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.IS_PATH.value] == 1) and (point2[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.IS_PATH.value] == 1):
            plt.plot(point_x, point_y, color = lineColour.SHORTEST_Line.value, linewidth = 2)
        elif (point1[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 1) and (point2[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 1):
            plt.plot(point_x, point_y, color = lineColour.MIDDLE_Line.value, linewidth = 1)
        else :
            plt.plot(point_x, point_y, color = lineColour.NORMAL_Line.value, linewidth = 1)


    plt.tight_layout()
    plt.show()

    wait = input("Press Enter to continue.")

def inputParameter(argv):
    filepath = defaultFilePath
    try:
        opts, args = getopt.getopt(argv, "hf:", ["help", "file="])
    except getopt.GetoptError:
        print('Error: displayPath.py -f <filepath>')
        print('   or: displayPath.py --file=<filepath>')
        sys.exit(2)

    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print('    displayPath.py -f <filepath>')
            print('or: displayPath.py --file=<filepath>')
            sys.exit()
        elif opt in ("-f", "--file"):
            filepath = arg



    print('filepath:', filepath)
    return filepath


if __name__ == "__main__":
    filepath = inputParameter(sys.argv[1:])
    main(filepath)







