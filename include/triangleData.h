/***********************************************************
 * \file      : triangleData.h
 * \brief     : This file contained a class that used to get
 *              the data from the software: Triangle
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/
#ifndef __TRIANGLEDATA_H__
#define __TRIANGLEDATA_H__

#include "stdint.h"
#include "string"

class node_t
{
public:
    uint32_t num;
    float x;
    float y;
    bool is_boundary;
};

class shape_t
{
public: 
    shape_t(){};
    ~shape_t(){delete[] node;};

    uint16_t num;        
    uint16_t nodes_num;
    node_t** node;
};

class triangleData
{
// public:
    // typedef struct
    // {
    //     uint32_t num;
    //     float x;
    //     float y;
    //     bool is_boundary;
    // }node_t;
    // typedef struct
    // {
    //     uint16_t points_num;        
    //     node_t* node;
    // }shape_t;
    
    

public:
    triangleData(/* args */);
    triangleData(std::string node_file, std::string ele_file);
    ~triangleData();

    void import(std::string node_file, std::string ele_file);

    void readNodes(std::string node_file);
    void readEles(std::string ele_file);

    void clearShape();


private:
    /* data */
    
    uint16_t nodeNum;
    node_t * nodeList;

    uint16_t shapeNum;
    shape_t * shapeList;


};











#endif