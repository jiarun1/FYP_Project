/***********************************************************
 * \file      : shortestPathAlgorithm.cpp
 * \brief     : This file contained a parent class for all 
 *              the shortest path algorithms
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/


#include "shortestPathAlgorithm.h"

shortestPathAlgorithm::shortestPathAlgorithm()
{

}


shortestPathAlgorithm::~shortestPathAlgorithm()
{

}


void shortestPathAlgorithm::calculateShortestPath()
{
    
}

void shortestPathAlgorithm::calculateShortestPath(uint16_t startPoint, uint16_t endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
}

void shortestPathAlgorithm::setAdjacencyMap(adjacencyMap* map)
{
    cost_map = map;
}


adjacencyMap* shortestPathAlgorithm::getAdjacencyMap()
{
    return cost_map;
}

uint16_t* shortestPathAlgorithm::getPath()
{
    return path;
}

uint16_t shortestPathAlgorithm::getPathLength()
{
    return path_length;
}

adjacencyMap::DISTANCE_ACCURACY shortestPathAlgorithm::getPathCost()
{
    return path_cost;
}