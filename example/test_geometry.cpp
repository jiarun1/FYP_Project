#include <iostream>
#include "mapGeometry.h"
#include "math.h"

using namespace std;


int main(int argv, char** argc)
{
    point_c A(0,0),B(4,0),C(2,3);

    triangle_c triangle(&A,&B,&C);
    triangle.initConnections();

    cout << "Area          : " << triangle.getArea() << endl;
    cout << "Maximum angle : " << cosh(triangle.maximumAngleCos()) << " radium OR " << cosh(triangle.maximumAngleCos())*360 / 2 / M_PI << " degree" << endl;

    point_c* fermat_point = triangle.getFermatPoint();
    point_c* centroid     = triangle.getMiddlePoint();
    point_c* circumcenter = triangle.getCircumCenter();
    point_c* orthocenter  = triangle.getOrthoCenter();

    cout <<  "Fermat point : (" << fermat_point->x << "," << fermat_point->y << ")" << endl;

    cout <<  "centroid     : (" << centroid->x     << "," << centroid->y     << ")" << endl;

    cout <<  "Circumcenter : (" << circumcenter->x << "," << circumcenter->y << ")" << endl;

    cout <<  "Orthocenter  : (" << orthocenter->x  << "," << orthocenter->y  << ")" << endl;


    return 0;
}