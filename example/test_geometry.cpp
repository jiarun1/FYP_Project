#include <iostream>
#include "mapGeometry.h"
#include "math.h"

using namespace std;


int main(int argv, char** argc)
{
    point_c A(20,15),B(0,0),C(-36,15);

    triangle_c triangle(&A,&B,&C);
    triangle.initConnections();

    cout << "Area          : " << triangle.getArea() << endl;
    cout << "Maximum angle : " << cosh(triangle.maximumAngleCos()) << " radium OR " << cosh(triangle.maximumAngleCos())*360 / 2 / M_PI << " degree" << endl;

    point_c* fermat_point = triangle.getFermatPoint();
    point_c* centroid     = triangle.getMiddlePoint();
    point_c* circumcenter = triangle.getCircumCenter();
    point_c* orthocenter  = triangle.getOrthoCenter();
    point_c* incenter     = triangle.getInCenter();

    if(fermat_point != nullptr)
    {
        cout <<  "Fermat point : (" << fermat_point->x << "," << fermat_point->y << ")" << endl;
    }

    if(centroid != nullptr)
    {
        cout <<  "centroid     : (" << centroid->x     << "," << centroid->y     << ")" << endl;
    }

    if(circumcenter != nullptr)
    {
        cout <<  "Circumcenter : (" << circumcenter->x << "," << circumcenter->y << ")" << endl;
    }

    if(orthocenter !=  nullptr)
    {
        cout <<  "Orthocenter  : (" << orthocenter->x  << "," << orthocenter->y  << ")" << endl;
    }

    if(incenter != nullptr)
    {
        cout <<  "Incenter     : (" << incenter->x     << "," << incenter->y     << ")" << endl;
    }


    

    

    


    return 0;
}