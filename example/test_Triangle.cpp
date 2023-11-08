#include <iostream>


#include "triangleCommand.h"
#include "triangleData.h"

int main(int argv, char** argc)
{
    // call the triangle with parameters
    triangleCommand triangle("../ThirdParty/triangle/triangle");

    triangle.addParameter("q",(int)30);
    triangle.addParameter("a",(float)0.005);
    triangle.addParameter("q",(int)50);
    triangle.clearParameter();

    triangle.call("../tests/triangle_test/Star.poly");
 
    // analyse the data from triangle
    // triangleData data;
    // data.import("../tests/triangle_test/Star.1.node", "../tests/triangle_test/Star.1.ele");

    

    return 0;
}