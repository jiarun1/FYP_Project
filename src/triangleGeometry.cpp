/***********************************************************
 * \file      : triangleGeometry.cpp
 * \brief     : This file contained a class for node
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/
#include "triangleGeometry.h"
#include "math.h"

float getPointDistance(node_c & node_x, node_c & node_y)
{
    float x_dif = node_x.x - node_y.x;
    float y_dif = node_x.y - node_y.y;

    return (sqrt(x_dif*x_dif+y_dif*y_dif));
}