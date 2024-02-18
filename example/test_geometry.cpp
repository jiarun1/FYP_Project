#include <iostream>
#include "mapGeometry.h"

using namespace std;


int main(int argv, char** argc)
{
    point_c A(1,2),B(5,3),C(2,6);

    triangle_c triangle(&A,&B,&C);
    triangle.initConnections();

    cout << triangle.getArea() << endl;

    point_c* fermat_point = triangle.getFermatPoint();

    cout << fermat_point->x << "," << fermat_point->y << endl;


    return 0;
}