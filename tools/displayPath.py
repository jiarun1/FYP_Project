import matplotlib.pyplot as plt
import tkinter as tk

from tkinter.filedialog import  askopenfilename

polyFilePath = "/home/jiarun/FYP_Project/code/tests/squareMapTest/Result.poly"

# tk.Tk().withdraw()
# polyFilePath = askopenfilename()
print(polyFilePath)


def read_poly_file(file_path):
    vertices = []
    polygons = []

    with open(file_path, 'r') as file:

        # Reading vertices
        num_vertices, num_dimension, num_property, num_boundary  = map(int, file.readline().split())
        for _ in range(num_vertices):
            line = file.readline().strip()
            parts = line.split()
            vertex_index = int(parts[0])
            coordinates = list([float(parts[1]), float(parts[2])])
            is_path = int(parts[3])
            vertices.append((vertex_index, coordinates, is_path))

        # Reading polygons
        num_polygons, num_boundary  = map(int, file.readline().split())
        for _ in range(num_polygons):
            line = file.readline().strip()
            parts = line.split()
            polygon_index = int(parts[0])
            vertex_indices = list([int(parts[1]), int(parts[2])])
            polygons.append((polygon_index, vertex_indices))

    return vertices, polygons


vertices, polygons = read_poly_file(polyFilePath)
# print(vertices)
# print(vertices[1][1][1])
# print(len(vertices))

plt.rc('font',family='Arial')
plt.ion()
plt.figure()

for i in range(len(vertices)):
    # print(vertices[i])

    if(vertices[i][2] == 1):
        plt.plot(vertices[i][1][0], vertices[i][1][1],'o', color = '#FF0000', markersize='4')
    else:
        plt.plot(vertices[i][1][0], vertices[i][1][1],'o', color = '#000000', markersize='4')


for i in range(len(polygons)):
    # print(polygons[i])

    point1 = vertices[polygons[i][1][0]]
    point2 = vertices[polygons[i][1][1]]

    point_x = [point1[1][0], point2[1][0]]
    point_y = [point1[1][1], point2[1][1]]

    if point1[2] == 1 and point2[2] == 1:
        plt.plot(point_x, point_y, color = '#FF0000')
    else :
        plt.plot(point_x, point_y, color = '#000000')

    
    
    



plt.tight_layout()
plt.show()

wait = input("Press Enter to continue.")








