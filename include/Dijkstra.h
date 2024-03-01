/***********************************************************
 * \file      : Dijkstra.h
 * \brief     : This file contained a class for Dijkstra 
 *              algorithm
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V5.2
***********************************************************/
#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "shortestPathAlgorithm.h"
#include "boost/heap/fibonacci_heap.hpp"

class pointInfoContainer
{
public:
    POINT_NUM_MAX label;
    fibonacci_heap<pointInfoContainer>::handle_type handle;
    double minDistance;
    pointInfoContainer* prePoint;
    POINT_NUM_MAX reachedBy; ///< use -1 to indicate the point has not reached by any start points

    pointInfoContainer(double i = DBL_MAX):
        minDistance(i), reachedBy(-1)
    {}

    ~pointInfoContainer(){};

    bool operator<(pointInfoContainer const & reference) const
    {
        // NOTE: Manually change the heap direction (default is Descending, but we require ascending)
        return minDistance > reference.minDistance;
    }
};


class Dijkstra : public shortestPathAlgorithm
{
public:
    Dijkstra(/* args */);
    virtual ~Dijkstra();

    void setAdjacencyMap(adjacencyMap* map);

    void calculateShortestPath();
    void calculateShortestPath(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint);
    

    void calculateShortestPathInParallel();
    void calculateShortestPathInParallel(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint);



private:
    /// @brief find the shortest path from the central point to all other point in the map
    /// @param centralPoint the central point of Dijkstra algorithm
    /// @note  this function would stop immediately when finding a point that has been reached
    void findPathFrom( POINT_NUM_MAX centralPoint);

    pointInfoContainer* pointInfos;
    
};





#endif