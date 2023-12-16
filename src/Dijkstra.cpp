/***********************************************************
 * \file      : Dijkstra.c
 * \brief     : This file contained a class for Dijkstra 
 *              algorithm
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V3.1
***********************************************************/
#include "Dijkstra.h"
#include "iostream"

#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>


Dijkstra::Dijkstra()
{
    path_point_num = 0;
}

using namespace boost::heap;

class heap_data
{
public:
    uint32_t label;
    fibonacci_heap<heap_data>::handle_type handle;
    double minDistance;
    heap_data* prePoint;
    bool reached;

    heap_data(double i = -1):
        minDistance(i), reached(false)
    {}

    ~heap_data(){};

    bool operator<(heap_data const & reference) const
    {
        // XXX: Manually change the heap direction (default is Descending, but we require ascending)
        return minDistance > reference.minDistance;
    }

    // bool operator>(heap_data const & reference) const
    // {
    //     return minDistance > reference.minDistance;
    // }
};


void Dijkstra::calculateShortestPath()
{
    uint16_t point_num = cost_map->getPointNum();

    heap_data* points_info = new heap_data[point_num];

    for(int i = 0; i < point_num; i++)
    {
        points_info[i].label = i;
    }

    boost::heap::fibonacci_heap<heap_data> points_dis_heap;

    uint16_t current_point = start_point;

    // initialize the start point
    points_info[current_point].minDistance = 0;

   
    do{
        points_info[current_point].reached = true;

        // based on the previous point, update all the point distance near the current one
        for(int i = 0; i < point_num; i++)
        {
            adjacencyMap::DISTANCE_ACCURACY cost = cost_map->getCost(current_point, i);
            if(cost == -1)
            {
                continue;
            }

            if(points_info[i].reached == true)
            {
                continue;
            }

            if(points_info[i].minDistance == -1)
            {
                points_info[i].minDistance = cost + points_info[current_point].minDistance;
                points_info[i].prePoint = &(points_info[current_point]);
                points_info[i].handle = points_dis_heap.push(points_info[i]);      
                continue;          
            }

            if(cost + points_info[current_point].minDistance < points_info[i].minDistance)
            {
                points_info[i].minDistance = cost + points_info[current_point].minDistance;
                points_info[i].prePoint = &(points_info[current_point]);

                // update the minimum points
                points_dis_heap.decrease(points_info[i].handle, points_info[i]);
            }            
        }

        //find the nearest point that has not been reached
        current_point = points_dis_heap.top().label;
        points_dis_heap.pop();

        // code for debug
        // while (!points_dis_heap.empty()) {
        //     std::cout << "(" << points_dis_heap.top().label<< "," << points_dis_heap.top().minDistance << ") "; // 5 4 3 2 1 0
        //     points_dis_heap.pop();
        // }
        // std::cout << std::endl;

    }while(current_point != end_point);

    /// update the path information

    // delete the previous pass data
    if(path_point_num != 0)
    {   
        delete[] path;
    }

    // used to count the length of the path
    uint16_t passed_point_num = 2;  //< count the start point and the end point
    uint16_t point_tmp = end_point;
    
    // contineous find the previous point until reach the start point
    while (points_info[point_tmp].prePoint->label != start_point)
    {
        passed_point_num ++;
        point_tmp = points_info[point_tmp].prePoint->label;
    }
    
    // store the path to the result
    path_point_num = passed_point_num;
    path_cost = points_info[end_point].minDistance;
    path = new uint16_t[path_point_num];

    path[path_point_num-1] = end_point;
    for(int i = path_point_num - 2; i > 0; i--)
    {
        path[i] = points_info[path[i+1]].prePoint->label;
    }
    path[0] = start_point;
}

void Dijkstra::calculateShortestPath(uint16_t startPoint, uint16_t endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
    calculateShortestPath();
}