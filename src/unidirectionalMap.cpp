/***********************************************************
 * @file      : unidirectionalMap.cpp
 * @brief     : This file is used to represent a unidirecti-
 *              onal map that is used to store the data for 
 *              the shortest path algorithm, it contains a 
 *              matrix for shortest path algorithm
 *              
 * @copyright : Copyright Jiarun LIU
 * @author    : Jiarun LIU
 * @version   : V0.1
***********************************************************/
#include "unidirectionalMap.h"

/**
 * @brief: constructure the map
*/
unidirectionalMap::unidirectionalMap():
    pointNum(0),distance(nullptr)
{

}

/**
 * @brief: initialize the class by entering the point number
*/
unidirectionalMap::unidirectionalMap(uint32_t point_num):
    pointNum(point_num)
{
    // create a pointNum * pointNum dimension matrix
    distance = new MAP_ACCURACY*[point_num];
    for(int i = 0; i < pointNum; i++)
    {
        distance[i] = new MAP_ACCURACY[point_num];
    }

    for(int i = 0; i < pointNum; i++)
    {
        for(int j = 0; j < pointNum; j++)
        {
            distance[i][j] = -1;
        }
    }
}

unidirectionalMap::unidirectionalMap(unidirectionalMap& copy)
{
    //TODO: add copying code
}

unidirectionalMap::~unidirectionalMap()
{

}



void unidirectionalMap::setDistance(int start_point, int end_point, unidirectionalMap::MAP_ACCURACY distance)
{
    
}











/**
 * @brief: Display the unidirectional map through the cout
*/
std::ostream& operator<< (std::ostream & out,const unidirectionalMap &out_map)
{
    out << "[ ";
    for(int i = 0; i < out_map.pointNum; i++)
    {
        for(int j = 0; j < out_map.pointNum; j++)
        {
            out << out_map.distance[i][j] << '\t';
        }

        if (i == out_map.pointNum - 1){
            out << " ]";
        } else {
            out << std::endl << "  ";
        }
    }

    return out;
}
