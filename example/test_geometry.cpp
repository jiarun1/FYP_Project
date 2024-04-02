#include <iostream>
#include "mapGeometry.h"
#include "math.h"
#include <libgen.h>
#include <chrono>

using namespace std;


int main(int argv, char** argc)
{
    point_c A(6,0),B(0,0),C(3,5.19);

    

    triangle_c triangle(&A,&B,&C);
    triangle.initConnections();
    
    cout << "Area          : " << triangle.getArea() << endl;
    cout << "Maximum angle : " << cosh(triangle.maximumAngleCos()) << " radium OR " << cosh(triangle.maximumAngleCos())*360 / 2 / M_PI << " degree" << endl;

    auto start_time = std::chrono::high_resolution_clock::now();
    point_c* fermat_point = triangle.getFermatPoint();
    auto fermat_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    start_time = std::chrono::high_resolution_clock::now();
    point_c* centroid     = triangle.getMiddlePoint();
    auto centroid_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    start_time = std::chrono::high_resolution_clock::now();
    point_c* circumcenter = triangle.getCircumCenter();
    auto circumcenter_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    start_time = std::chrono::high_resolution_clock::now();
    point_c* orthocenter  = triangle.getOrthoCenter();
    auto orthocenter_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    start_time = std::chrono::high_resolution_clock::now();
    point_c* incenter     = triangle.getInCenter();
    auto incenter_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);


    if(fermat_point != nullptr)
    {
        cout <<  "Fermat point : (" << fermat_point->x << "," << fermat_point->y << ")" << "\t\t   Algorithm Time Cost: " << fermat_time.count() << endl;
    }

    if(centroid != nullptr)
    {
        cout <<  "centroid     : (" << centroid->x     << "," << centroid->y     << ")" << "\t\t   Algorithm Time Cost: " << centroid_time.count() << endl;
    }

    if(circumcenter != nullptr)
    {
        cout <<  "Circumcenter : (" << circumcenter->x << "," << circumcenter->y << ")" << "\t\t   Algorithm Time Cost: " << circumcenter_time.count() << endl;
    }

    if(orthocenter !=  nullptr)
    {
        cout <<  "Orthocenter  : (" << orthocenter->x  << "," << orthocenter->y  << ")" << "\t\t   Algorithm Time Cost: " << orthocenter_time.count() << endl;
    }

    if(incenter != nullptr)
    {
        cout <<  "Incenter     : (" << incenter->x     << "," << incenter->y     << ")" << "\t\t   Algorithm Time Cost: " << incenter_time.count() << endl;
    }


    

    

    


    return 0;
}