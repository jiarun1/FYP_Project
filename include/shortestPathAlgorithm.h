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

    virtual void setAdjacencyMap(adjacencyMap* map);
    adjacencyMap* getAdjacencyMap();

    PATH_POINT_NUM_MAX* getPath();
    PATH_POINT_NUM_MAX getPathPointNum();

    ACCURACY getPathCost();

    /**
     * @brief export the shortest path with all the point in the map to .poly file
     * @param file_path the path of the .poly file 
    */
    void exportToPolyFile(std::string file_path);

    
    /**
     * @brief export the shortest path with only adjacent triangle to the .poly file
     * @param file_path the path of the .poly file 
     */
    void exportAdjacentToPolyFile(std::string file_path);

    /**
     * @brief: Display the shortest path through the cout
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