#include <iostream>
#include "mapGeometry.h"

using namespace std;


int main(int argv, char** argc)
{
    point_c A(3,2),B(1,4),C(5,4);

    triangle_c triangle(&A,&B,&C);
    triangle.initConnections();

    cout << triangle.getArea() << endl;

    point_c* fermat_point = triangle.getFermatPoint();
    point_c* circumcenter = triangle.getCircumCenter();
    point_c* centroid     = triangle.getMiddlePoint();

    cout <<  "Fermat point : (" << fermat_point->x << "," << fermat_point->y << ")" << endl;

    cout <<  "centroid     : (" << centroid->x     << "," << centroid->y     << ")" << endl;

    cout <<  "Circumcenter : (" << circumcenter->x << "," << circumcenter->y << ")" << endl;


    return 0;
}