import os
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
    FERMAT_POINT = '#6495ED'
    CIRCUMCENTER = '#6495ED'
    ORTHOCENTER  = '#6495ED'
    INCENTER     = '#6495ED'

class lineColour(Enum):
    NORMAL_Line = "#C0C0C0"
    SHORTEST_Line = '#FFA500'
    MIDDLE_Line = '#6495ED'
    FERMAT_Line = '#00F000'

#polyFilePath = "/home/jiarun/FYP_Project/code/tests/squareMapTest/Result.poly"
defaultFilePath = "/home/jiarun/Desktop/FYP/code/tests/squareMapTest/Result.poly"

def remove_comments(line):
    comments_start = line.strip().find('#')
    return line[:comments_start]

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
# def read_poly_file(file_path):
#     vertices = []
#     polygons = []

#     with open(file_path, 'r') as file:

#         # Reading vertices
#         num_vertices, num_dimension, num_property, num_boundary  = map(int, file.readline().split())
#         for _ in range(num_vertices):
#             line = file.readline().strip()

#             # ignore the comment line
#             if(line.find("#") != -1):
#                 continue

#             parts = line.split()
#             # if(parts.count() < (1 + num_dimension + num_property + num_boundary)):
#             #     print("Reading verticles failed")
#             #     exit()

#             item_count = 0
#             coordinates = list()
#             propertys = list()

#             vertex_index = int(parts[item_count])
#             for _ in range(num_dimension):
#                 item_count += 1
#                 coordinates.append(float(parts[item_count]))

#             for _ in range(num_property):
#                 item_count += 1
#                 propertys.append(int(parts[item_count]))
                
#             vertices.append((vertex_index, coordinates, propertys))

#         # Reading polygons
#         num_polygons, num_boundary  = map(int, file.readline().split())
#         for _ in range(num_polygons):
#             line = file.readline().strip()

#             # ignore the comment line
#             if(line.find("#") != -1):
#                 continue

#             parts = line.split()
#             polygon_index = int(parts[0])
#             vertex_indices = list([int(parts[1]), int(parts[2])])
#             polygons.append((polygon_index, vertex_indices))

#     return vertices, polygons

def main(polyPath):

    print(polyPath)
    # tk.Tk().withdraw()
    # polyFilePath = askopenfilename()
    vertices, polygons, holes = read_poly_file(polyPath)
    # print(vertices)
    # print(vertices[1][1][1])
    # print(len(vertices))

    plt.rc('font',family='Arial')
    #plt.ion()
    plt.figure("Path Display",figsize=(18/2.54,9/2.54),dpi=200)
    # plt.figure("Path Display",figsize=(18/2.54,9/2.54),dpi=200)
    plt.axis("off")


    h1, = plt.plot([-10, -8],[5, -3], color = "#B28DFF", linewidth = 2)
    plt.plot([-6, -4],[-3, -1], color = "#B28DFF", linewidth = 2)
    plt.plot([0, 2],[-1, -3], color = "#B28DFF", linewidth = 2)
    plt.plot([4, 6],[-3, 3], color = "#B28DFF", linewidth = 2)
    plt.plot([8, 10],[3, -5], color = "#B28DFF", linewidth = 2)

    # plot polygens
    for i in range(len(polygons)):
        # print(polygons[i])

        # this 2 value is the overall point detail list
        point1 = vertices[polygons[i][1][0] - 1]
        point2 = vertices[polygons[i][1][1] - 1]

        # this 2 value is the x value list of the 2 points and the y value list of the points
        point_x = [point1[verticleParam_e.COORDINATE.value][0], point2[verticleParam_e.COORDINATE.value][0]]
        point_y = [point1[verticleParam_e.COORDINATE.value][1], point2[verticleParam_e.COORDINATE.value][1]]

        if (point1[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.IS_PATH.value] == 1) and (point2[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.IS_PATH.value] == 1):
            h2, = plt.plot(point_x, point_y, color = lineColour.SHORTEST_Line.value, linewidth = 2)
        # elif (point1[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 1) and (point2[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 1):
        #     plt.plot(point_x, point_y, color = lineColour.MIDDLE_Line.value, linewidth = 1)
        # elif (point1[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 2) and (point2[verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 2):
        #     plt.plot(point_x, point_y, color = lineColour.MIDDLE_Line.value, linewidth = 1)
        else:
            plt.plot(point_x, point_y, color = lineColour.NORMAL_Line.value, linewidth = 1)


    # plot vertices
    for i in range(len(vertices)):
        # print(vertices[i])
    
        if(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 1):
            # display the middle point
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.MIDDLE_POINT.value, markersize='3')
        elif(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 2):
            # display the fermat point
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.FERMAT_POINT.value, markersize='3')
        elif(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 3):
            # display the circumcenter point
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.CIRCUMCENTER.value, markersize='3')
        elif(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 4):
            # display the orthocenter point
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.ORTHOCENTER.value, markersize='3')
        elif(vertices[i][verticleParam_e.PROPORITIES.value][verticleProporityParam_e.POINT_TYPE.value] == 5):
            # display the orthocenter point
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.INCENTER.value, markersize='3')
        else:
            plt.plot(vertices[i][verticleParam_e.COORDINATE.value][0], vertices[i][verticleParam_e.COORDINATE.value][1],'o', color = pointColour.NORMAL_POINT.value, markersize='3')


    plt.legend([h1,h2],["Real Shortest Path", "Calculated Shortest Path"], loc='upper center', bbox_to_anchor=(0.5, 1.15), ncol=2)    

    plt.tight_layout()
    plt.show()

    wait = input("Press Enter to continue.")


if __name__ == "__main__":
    os.system("cd ../../bin && ./ShortestPathPlanning -f ../tests/mazeMapTest/mazeMapTest.poly -s 1 -e 22 -a 1")
    filepath = "../../tests/mazeMapTest/Result.poly"
    main(filepath)







