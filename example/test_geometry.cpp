#include <iostream>
#include "mapGeometry.h"
#include "math.h"
#include <libgen.h>
#include <chrono>

using namespace std;

uint64_t test_count = 100000;

int main(int argv, char** argc)
{
    // point_c A(6,0),B(0,0),C(3,5.19);

    // triangle_c triangle(&A,&B,&C);
    // triangle.initConnections();

    triangle_c* triangles[test_count];
    for(int i = 0; i < test_count; i ++)
    {
        point_c* A = new point_c(6,0);
        point_c* B = new point_c(0,0);
        point_c* C = new point_c(3.5,19);

        triangles[i] = new triangle_c(A,B,C);
        triangles[i]->initConnections();
    }

    auto triangle = *triangles[0];
    
    cout << "Area          : " << triangle.getArea() << endl;
    cout << "Maximum angle : " << cosh(triangle.maximumAngleCos()) << " radium OR " << cosh(triangle.maximumAngleCos())*360 / 2 / M_PI << " degree" << endl;


    point_c *fermat_point, *centroid, *circumcenter, *orthocenter, *incenter;

    auto start_time = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < test_count; i++)
    {
        fermat_point = triangles[i]->getFermatPoint();
    }
    auto fermat_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    start_time = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < test_count; i++)
    {
        centroid = triangles[i]->getMiddlePoint();
    }
    auto centroid_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    start_time = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < test_count; i++)
    {
        circumcenter = triangles[i]->getCircumCenter();
    }
    auto circumcenter_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    start_time = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < test_count; i++)
    {
        orthocenter = triangles[i]->getOrthoCenter();
    }
    auto orthocenter_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    start_time = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < test_count; i++)
    {
        incenter = triangles[i]->getInCenter();
    }
    auto incenter_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);

    cout << "The Time is for calcultion of " << test_count << " points in triangle\n";

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