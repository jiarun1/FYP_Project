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
    POINT_NUM_MAX label;
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
};


void Dijkstra::calculateShortestPath()
{
    POINT_NUM_MAX point_num = cost_map->getPointNum();

    heap_data* points_info = new heap_data[point_num];

    for(int i = 0; i < point_num; i++)
    {
        points_info[i].label = i + 1;
    }

    boost::heap::fibonacci_heap<heap_data> points_dis_heap;

    pointInfo_c* current_point = cost_map->findPoint(start_point);

    // initialize the start point
    points_info[current_point->num - 1].minDistance = 0;

   
    do{
        points_info[current_point->num - 1].reached = true;

        // based on the previous point, update all the point distance near the current one
        auto current_connection_path = cost_map->getConnections(current_point);

        for(int i = 0; i < current_connection_path.size(); i++)
        {
            auto next_point = current_connection_path[i]->getAnotherNode(current_point);
            auto connection_cost = current_connection_path[i]->distance;

            // if the point is reached, goto the next point
            if(points_info[next_point->num - 1].reached == true){
                continue;
            }

            if(points_info[next_point->num - 1].minDistance == -1)
            {
                points_info[next_point->num - 1].minDistance = connection_cost + points_info[current_point->num - 1].minDistance;
                points_info[next_point->num - 1].prePoint = &(points_info[current_point->num - 1]);
                points_info[next_point->num - 1].handle = points_dis_heap.push(points_info[next_point->num - 1]);      
                continue;          
            }

            if(connection_cost + points_info[current_point->num - 1].minDistance < points_info[next_point->num - 1].minDistance)
            {
                points_info[next_point->num - 1].minDistance = connection_cost + points_info[current_point->num - 1].minDistance;
                points_info[next_point->num - 1].prePoint = &(points_info[current_point->num - 1]);

                // update the minimum points
                points_dis_heap.decrease(points_info[next_point->num - 1].handle, points_info[next_point->num - 1]);
            }      

        }

        //find the nearest point that has not been reached
        current_point = cost_map->findPoint(points_dis_heap.top().label);
        points_dis_heap.pop();

        // code for debug
        // while (!points_dis_heap.empty()) {
        //     std::cout << "(" << points_dis_heap.top().label<< "," << points_dis_heap.top().minDistance << ") "; // 5 4 3 2 1 0
        //     points_dis_heap.pop();
        // }
        // std::cout << std::endl;

    }while(current_point->num != end_point);

    /// update the path information

    // delete the previous pass data
    if(path_point_num != 0)
    {   
        delete[] path;
    }

    // used to count the length of the path
    PATH_POINT_NUM_MAX passed_point_num = 2;  //< count the start point and the end point
    POINT_NUM_MAX point_tmp = end_point;
    
    // contineous find the previous point until reach the start point
    while (points_info[point_tmp - 1].prePoint->label != start_point)
    {
        passed_point_num ++;
        point_tmp = points_info[point_tmp - 1].prePoint->label;
    }
    
    // store the path to the result
    path_point_num = passed_point_num;
    path_cost = points_info[end_point - 1].minDistance;
    path = new PATH_POINT_NUM_MAX[path_point_num];

    path[path_point_num-1] = end_point;
    for(int i = path_point_num - 2; i > 0; i--)
    {
        path[i] = points_info[path[i+1]-1].prePoint->label;
    }
    path[0] = start_point;
}

void Dijkstra::calculateShortestPath(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
    calculateShortestPath();
}