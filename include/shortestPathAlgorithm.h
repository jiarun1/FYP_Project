/***********************************************************
 * \file      : shortestPathAlgorithm.h
 * \brief     : This file contained a parent class for all 
 *              the shortest path algorithms
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/
#ifndef __SHORTESTPATHALGORITHM_H__
#define __SHORTESTPATHALGORITHM_H__

#include "adjacencyMap.h"
#include "stdint.h"

class shortestPathAlgorithm
{
public:
    shortestPathAlgorithm();

    virtual ~shortestPathAlgorithm();

    virtual void calculateShortestPath();
    virtual void calculateShortestPath(uint16_t startPoint, uint16_t endPoint);

    void setAdjacencyMap(adjacencyMap* map);
    adjacencyMap* getAdjacencyMap();

    uint16_t* getPath();
    uint16_t getPathLength();

    adjacencyMap::DISTANCE_ACCURACY getPathCost();

protected:
    
    /// @brief used to store the cost data for the map
    adjacencyMap* cost_map;
    
    /// @brief the calculated shortest path
    uint16_t* path; 

    /// @brief the number of points in the path
    uint16_t path_length;

    /// @brief the start point for the path
    uint16_t start_point;

    /// @brief the end point for the path
    uint16_t end_point;

    /// @brief the length of the shortest path
    adjacencyMap::DISTANCE_ACCURACY path_cost;
    

};
#endif