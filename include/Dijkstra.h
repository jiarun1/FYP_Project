/***********************************************************
 * \file      : Dijkstra.h
 * \brief     : This file contained a class for Dijkstra 
 *              algorithm
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V3.1
***********************************************************/
#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "shortestPathAlgorithm.h"

class Dijkstra : public shortestPathAlgorithm
{
public:
    Dijkstra(/* args */);
    ~Dijkstra()
    {

    }

    void calculateShortestPath();
    void calculateShortestPath(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint);

private:
    
};





#endif