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

using namespace boost::heap;

class pointInfoContainer;

class pointInfoHeap
{
public:
    pointInfoContainer* parent;
    fibonacci_heap<pointInfoHeap>::handle_type handle;
    double minDistance;
    pointInfoHeap *prePoint;


    pointInfoHeap():
        minDistance(-1),prePoint(nullptr),parent(nullptr)
    {
        
    }

    pointInfoHeap(pointInfoContainer* Parent):
        minDistance(-1),prePoint(nullptr),parent(Parent)
    {
        
    }

    ~pointInfoHeap(){};

    bool operator<(pointInfoHeap const & reference) const
    {
        // NOTE: Manually change the heap direction (default is Descending, but we require ascending)
        return minDistance > reference.minDistance;
    }
};

class pointInfoContainer
{
public:
    int thread_number;
    pointInfoHeap* heap_data;
    POINT_NUM_MAX label;
    POINT_NUM_MAX reachedBy; ///< use -1 to indicate the point has not reached by any start points

    pointInfoContainer():
        heap_data(nullptr),reachedBy(0)
    {

    }

    ~pointInfoContainer(){
        // prevent delete twice
        // if(heap_data != nullptr)
        // {
        //     delete[] heap_data;
        //     heap_data = nullptr;
        // }
    }


    void init(int num_thread)
    {
        thread_number = (num_thread);
        heap_data = new pointInfoHeap[num_thread];
        std::fill(heap_data, heap_data+num_thread, this);
    }

    void release()
    {
        delete[] heap_data;
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


    void calculateShortestPath_Bidirectional();
    void calculateShortestPath_Bidirectional(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint);
    

    void calculateShortestPathInParallel();
    void calculateShortestPathInParallel(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint);



private:

    /// @brief find the shortest path from the central point to all other point in the map
    /// @param centralPoint the central point of Dijkstra algorithm
    /// @note  this function would stop immediately when finding a point that has been reached
    void findPathFrom(pointInfoContainer* points_info, point_c* reached_point, POINT_NUM_MAX centralPoint, int thread_label);


    // pointInfoContainer* pointInfos;
    
};





#endif