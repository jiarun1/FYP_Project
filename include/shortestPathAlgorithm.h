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
    virtual void calculateShortestPath(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint);

    void setAdjacencyMap(adjacencyMap* map);
    adjacencyMap* getAdjacencyMap();

    PATH_POINT_NUM_MAX* getPath();
    PATH_POINT_NUM_MAX getPathPointNum();

    ACCURACY getPathCost();

    void exportToPolyFile(std::string file_path);

    /**
     * @brief: Display the shprtest path through the cout
    */
    friend std::ostream& operator<< (std::ostream & out,const shortestPathAlgorithm &outPath);

protected:
    
    /// @brief used to store the cost data for the map
    adjacencyMap* cost_map;
    
    /// @brief the calculated shortest path
    PATH_POINT_NUM_MAX* path; 

    /// @brief the number of points in the path
    PATH_POINT_NUM_MAX path_point_num;

    /// @brief the start point for the path
    POINT_NUM_MAX start_point;

    /// @brief the end point for the path
    POINT_NUM_MAX end_point;

    /// @brief the length of the shortest path
    ACCURACY path_cost;
    

};
#endif