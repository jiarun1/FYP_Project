/***********************************************************
 * \file      : Dijkstra.h
 * \brief     : This file contained a class for Dijkstra 
 *              algorithm
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V1.1
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
    void calculateShortestPath(uint16_t startPoint, uint16_t endPoint);

private:
    
};





#endif