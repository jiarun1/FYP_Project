#include <iostream>
#include "adjacencyMap.h"

#include "triangleCommand.h"
#include "triangleData.h"
#include "adjacencyMap.h"
#include "Dijkstra.h"

static const std::string TrianglePath = "../ThirdParty/triangle/triangle";


static const std::string MapPath = "../tests/squareMapTest/";
static const std::string MapName = "squareMapTest";
static const int START_POINT = 1;
static const int END_POINT = 3;


int main(int argv, char** argc)
{
    // call the triangle
    triangleCommand triangle(TrianglePath);

    triangle.setParameter("a", (float)3);

    triangle.call(MapPath + MapName +".poly");
 
    // analyse the data from triangle
    triangleData data;
    data.import(MapPath + MapName + ".1.node", MapPath + MapName + ".1.ele");

    adjacencyMap map;
    data.toAdjacencyMap(&map);

    // Dijkstra algorithm start

    Dijkstra path_planing;
    path_planing.setAdjacencyMap(&map);
    path_planing.calculateShortestPath(START_POINT, END_POINT);

    path_planing.exportToPolyFile(MapPath + "Result.poly");

    std::cout << "Shortest Distance: " << float(path_planing.getPathCost()) << std::endl;
    std::cout << path_planing;

    system("/bin/python3 ../tools/displayPath.py");

    return 0;
}


