#include <iostream>


#include "triangleCommand.h"
#include "triangleData.h"
#include "adjacencyMap.h"

int main(int argv, char** argc)
{
    // call the triangle with parameters
    triangleCommand triangle("../ThirdParty/triangle/triangle");

    triangle.setParameter("q",(int)30);
    triangle.setParameter("a",(float)0.005);
    triangle.setParameter("q",(int)50);
    triangle.clearParameter();

    triangle.call("../tests/triangle_test/Star.poly");
 
    // analyse the data from triangle
    triangleData data;
    data.import("../tests/triangle_test/Star.1.node", "../tests/triangle_test/Star.1.ele");

    adjacencyMap map;
    data.toAdjacencyMap(&map);
    
    std::cout << map << std::endl;

    return 0;
}