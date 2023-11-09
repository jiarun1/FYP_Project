/***********************************************************
 * \file      : triangleGeometry.h
 * \brief     : This file contained several class used to store
 *              the data for triangle software and some used geometry
 *              function in this project
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/
#ifndef __TRIANGLEGEOMETRY_H__
#define __GEOMETRY_H__

#include <stdint.h>

class node_c
{
public:
    uint32_t num;
    float x;
    float y;
    bool is_boundary;
};


class shape_c
{
public: 
    shape_c(){};
    ~shape_c(){delete[] node;};

    uint16_t num;        
    uint16_t nodes_num;
    node_c** node;
};

#endif
