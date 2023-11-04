/***********************************************************
 * @file      : costMap.cpp
 * @brief     : This file is used to represent a unidirecti-
 *              onal map that is used to store the data for 
 *              the shortest path algorithm, it contains a 
 *              matrix for shortest path algorithm
 *              
 * @copyright : Copyright Jiarun LIU
 * @author    : Jiarun LIU
 * @version   : V0.1
***********************************************************/
#include "costMap.h"

/**
 * @brief: constructure the map
*/
costMap::costMap():
    point_num(0),cost(nullptr)
{

}

/**
 * @brief: initialize the class by entering the point number
*/
costMap::costMap(uint32_t point_num):
    point_num(point_num)
{
    // create a point_num * point_num dimension matrix
    cost = new DISTANCE_ACCURACY*[point_num];
    for(int i = 0; i < point_num; i++)
    {
        cost[i] = new DISTANCE_ACCURACY[point_num];
    }

    for(int i = 0; i < point_num; i++)
    {
        for(int j = 0; j < point_num; j++)
        {
            cost[i][j] = -1;
        }
    }
}

costMap::costMap(costMap& copy)
{
    //TODO: add copying code
}

costMap::~costMap()
{

}



void costMap::setCost(int start_point, int end_point, costMap::DISTANCE_ACCURACY dis, costMap::WEIGHT_ACCURACY weight)
{
    cost[start_point][end_point] = dis;
}

costMap::DISTANCE_ACCURACY costMap::getCost(int start_point, int end_point)
{
    return cost[start_point][end_point];
}

uint32_t costMap::getPointNum(void)
{
    return point_num;
}









/**
 * @brief: Display the unidirectional map through the cout
*/
std::ostream& operator<< (std::ostream & out,const costMap &out_map)
{
    out << "[ ";
    for(int i = 0; i < out_map.point_num; i++)
    {
        for(int j = 0; j < out_map.point_num; j++)
        {
            out << out_map.cost[i][j] << '\t';
        }

        if (i == out_map.point_num - 1){
            out << " ]";
        } else {
            out << std::endl << "  ";
        }
    }

    return out;
}
