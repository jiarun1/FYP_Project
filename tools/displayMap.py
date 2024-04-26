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
    NORMAL_POINT = "#696969"
    SHORTEST_POINT = '#FFA500'
    MIDDLE_POINT = '#6495ED'
    FERMAT_POINT = '#00F000'

class lineColour(Enum):
    NORMAL_Line = "#C0C0C0"
    SHORTEST_Line = '#FFA500'
    MIDDLE_Line = '#6495ED'
    FERMAT_Line = '#00F000'

#polyFilePath = "/home/jiarun/FYP_Project/code/tests/squareMapTest/Result.poly"
defaultFilePath = "../tests/squareMapTest/Result.poly"

def remove_comments(line):
    comments_start = line.strip().find('#')
    return line[:comments_start]

# def find_polygons(segments):

#     def adjacent_point_in_segments(point, segment):
#         if(segment[1][0] == point):
#             return segment[1][1]
#         elif (segment[1][1] == point):
#             return segment[1][0]
#         else:
#             return False
        

#     for i in range(len(segments)):

        

def read_poly_file(file_path):
    vertices = []
    segments = []
    holes = []

    with open(file_path, 'r') as file:

        ################################################################################
        # Reading vertices
        line = file.readline()
        line = remove_comments(line)
        while(len(line) <= 0):
            line = file.readline()
            line = remove_comments(line)

        num_vertices, num_dimension, num_property, num_boundary  = map(int, line.split())
        for _ in range(num_vertices):
            # ignore the comment line
            line = file.readline()  
            line = remove_comments(line)
            while(len(line) <= 0):
                line = file.readline()
                line = remove_comments(line)

            parts = line.strip().split()
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



        ################################################################################
        # Reading segments
        # remove the comments between 2 parts
        line = file.readline()  
        line = remove_comments(line)
        while(len(line) <= 0):
            line = file.readline()
            line = remove_comments(line)
        num_segments, num_boundary  = map(int, line.split())
        for _ in range(num_segments):
            # ignore the comment line
            line = file.readline()  
            line = remove_comments(line)
            while(len(line) <= 0):
                line = file.readline()
                line = remove_comments(line)

            parts = line.strip().split()

            segment_index = int(parts[0])
            vertex_indices = list([int(parts[1]), int(parts[2])])
            segments.append((segment_index, vertex_indices))

        ################################################################################
        # Reading holes
        # remove the comments between 2 parts
        line = file.readline()  
        line = remove_comments(line)
        while(len(line) <= 0):
            line = file.readline()
            line = remove_comments(line)
        num_holes, = map(int, line.split())
        for _ in range(num_holes):
            # ignore the comment line
            line = file.readline()  
            line = remove_comments(line)
            while(len(line) <= 0):
                line = file.readline()
                line = remove_comments(line)

            parts = line.strip().split()

            hole_index = int(parts[0])
            hole_coordinates = list([int(parts[1]), int(parts[2])])
            holes.append((hole_index, hole_coordinates))

    return vertices, segments, holes

def main(polyPath):

    print(polyPath)
    vertices, segments, holes = read_poly_file(polyPath)



    plt.rc('font',family='Arial')
    plt.figure("Map Display",figsize=(9/2.54,9/2.54),dpi=200)
    # plt.figure("Map Display",figsize=(18/2.54,9/2.54),dpi=200)

    plt.axis("off")

    # plot polygens
    for i in range(len(segments)):
        # print(polygons[i])

        # this 2 value is the overall point detail list
        point1 = vertices[segments[i][1][0] - 1]
        point2 = vertices[segments[i][1][1] - 1]

        # this 2 value is the x value list of the 2 points and the y value list of the points
        point_x = [point1[verticleParam_e.COORDINATE.value][0], point2[verticleParam_e.COORDINATE.value][0]]
        point_y = [point1[verticleParam_e.COORDINATE.value][1], point2[verticleParam_e.COORDINATE.value][1]]

        plt.plot(point_x, point_y, color = lineColour.NORMAL_Line.value, linewidth = 1)


    # plot vertices
    for i in range(len(vertices)):
        if(len(vertices[i][verticleParam_e.PROPORITIES.value]) == 0):
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.NORMAL_POINT.value, markersize='3')
        else:
            # print(vertices[i])
            if(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 1):
                plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.MIDDLE_POINT.value, markersize='3')
            elif(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 2):
                plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.FERMAT_POINT.value, markersize='3')
            else:
                plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.NORMAL_POINT.value, markersize='3')



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







