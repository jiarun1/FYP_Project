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

#include "triangleGeometry.h"
#include "adjacencyMap.h" ///< used to convert the data to adjacencyMap



class triangleData
{
public:
    triangleData(/* args */);
    triangleData(std::string node_file, std::string ele_file);
    ~triangleData();

    void import(std::string node_file, std::string ele_file);

    void readNodes(std::string node_file);
    void readEles(std::string ele_file);

    void clearShape();

    void toAdjacencyMap(adjacencyMap* map);


private:
    /* data */
    
    uint16_t nodeNum;
    node_c * nodeList;

    uint16_t shapeNum;
    shape_c * shapeList;


};











#endif