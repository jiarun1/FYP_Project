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
#include "fstream"

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
    uint16_t getPathPointNum();

    adjacencyMap::DISTANCE_ACCURACY getPathCost();

    void exportToPolyFile(std::string file_path);

    /**
     * @brief: Display the shprtest path through the cout
    */
    friend std::ostream& operator<< (std::ostream & out,const shortestPathAlgorithm &outPath);

protected:
    
    /// @brief used to store the cost data for the map
    adjacencyMap* cost_map;
    
    /// @brief the calculated shortest path
    uint16_t* path; 

    /// @brief the number of points in the path
    uint16_t path_point_num;

    /// @brief the start point for the path
    uint16_t start_point;

    /// @brief the end point for the path
    uint16_t end_point;

    /// @brief the length of the shortest path
    adjacencyMap::DISTANCE_ACCURACY path_cost;
    

};
#endif