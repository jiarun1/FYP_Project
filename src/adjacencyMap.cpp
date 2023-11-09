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
#include "adjacencyMap.h"

/**
 * @brief: constructure the map
*/
adjacencyMap::adjacencyMap():
    pointNum(0),cost(nullptr)
{

}

/**
 * @brief: initialize the class by entering the point number
*/
adjacencyMap::adjacencyMap(uint32_t point_num):
    pointNum(0),cost(nullptr)
{
    setPointNum(point_num);
}

adjacencyMap::adjacencyMap(adjacencyMap& copy)
{
    //TODO: add copying code
}

adjacencyMap::~adjacencyMap()
{
    clear();
}

void adjacencyMap::clear()
{
    if(pointNum == 0){
        return;
    }


    for(int i = 0; i < pointNum; i++)
    {
        delete[] cost[i];
    }
    delete[] cost;
}

void adjacencyMap::setCost(int start_point, int end_point, adjacencyMap::DISTANCE_ACCURACY dis, adjacencyMap::WEIGHT_ACCURACY weight)
{
    cost[start_point][end_point] = dis;
}

adjacencyMap::DISTANCE_ACCURACY adjacencyMap::getCost(int start_point, int end_point)
{
    return cost[start_point][end_point];
}

void adjacencyMap::setPointNum(uint32_t point_num)
{
    clear();
    pointNum = point_num;
    // create a pointNum * pointNum dimension matrix
    cost = new DISTANCE_ACCURACY*[pointNum];
    for(int i = 0; i < pointNum; i++)
    {
        cost[i] = new DISTANCE_ACCURACY[pointNum];
    }

    for(int i = 0; i < pointNum; i++)
    {
        for(int j = 0; j < pointNum; j++)
        {
            cost[i][j] = -1;
        }
    }
}

uint32_t adjacencyMap::getPointNum(void)
{
    return pointNum;
}









/**
 * @brief: Display the unidirectional map through the cout
*/
std::ostream& operator<< (std::ostream & out,const adjacencyMap &out_map)
{
    out << "[ ";
    for(int i = 0; i < out_map.pointNum; i++)
    {
        for(int j = 0; j < out_map.pointNum; j++)
        {
            out << out_map.cost[i][j] << '\t';
        }

        if (i == out_map.pointNum - 1){
            out << " ]";
        } else {
            out << std::endl << "  ";
        }
    }

    return out;
}
