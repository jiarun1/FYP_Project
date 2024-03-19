/***********************************************************
 * \file      : Dijkstra.c
 * \brief     : This file contained a class for Dijkstra 
 *              algorithm (contain the multi thread code)
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V5.2
***********************************************************/
#include "Dijkstra.h"
#include "iostream"

#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>
#include <thread>


Dijkstra::Dijkstra():
    pointInfos(nullptr),
    shortestPathAlgorithm()
{

}

Dijkstra::~Dijkstra()
{
    if(pointInfos != nullptr)
    {
        delete[] pointInfos;
    }
}


void Dijkstra::setAdjacencyMap(adjacencyMap* map)
{
    cost_map = map;
    pointInfos = new pointInfoContainer[map->getPointNum()];

    // give all the point the label of it
    for(int i = 0; i < map->getPointNum(); i++)
    {
        pointInfos[i].label = i + 1;
    }
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
        // NOTE: Manually change the heap direction (default is Descending, but we require ascending)
        return minDistance > reference.minDistance;
    }
};


void Dijkstra::calculateShortestPath(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
    calculateShortestPath();
} 
void Dijkstra::calculateShortestPath()
{
    POINT_NUM_MAX point_num = cost_map->getPointNum();

    heap_data* points_info = new heap_data[point_num];

    for(int i = 0; i < point_num; i++)
    {
        points_info[i].label = i + 1;
    }

    boost::heap::fibonacci_heap<heap_data> points_dis_heap;

    point_c* current_point = cost_map->findPoint(start_point);

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


void Dijkstra::calculateShortestPathInParallel(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
    calculateShortestPathInParallel();
}

void Dijkstra::calculateShortestPathInParallel()
{
    //TODO: add the code to enable the algorithm run multiple times

    // pointInfos[start_point - 1].reachedBy = start_point;
    // pointInfos[end_point - 1].reachedBy = end_point;
    
    // std::thread startThread(findPathFrom, start_point);
    // std::thread endThread(findPathFrom, end_point);

    // startThread.join();
    // endThread.join();





    
}





void Dijkstra::findPathFrom(POINT_NUM_MAX centralPoint)
{

    boost::heap::fibonacci_heap<pointInfoContainer> points_dis_heap;

    point_c* current_point = cost_map->findPoint(start_point);

    // initialize the start point
    pointInfos[current_point->num - 1].minDistance = 0;
    pointInfos[current_point->num - 1].reachedBy = centralPoint;

   
    do{
        pointInfos[current_point->num - 1].reachedBy = centralPoint;

        // based on the previous point, update all the point distance near the current one
        auto current_connection_path = cost_map->getConnections(current_point);

        for(int i = 0; i < current_connection_path.size(); i++)
        {
            auto next_point = current_connection_path[i]->getAnotherNode(current_point);
            auto connection_cost = current_connection_path[i]->distance;

            // if the point is reached, goto the next point
            if(pointInfos[next_point->num - 1].reachedBy == centralPoint){
                continue;
            }

            if(pointInfos[next_point->num - 1].minDistance == -1)
            {
                pointInfos[next_point->num - 1].minDistance = connection_cost + pointInfos[current_point->num - 1].minDistance;
                pointInfos[next_point->num - 1].prePoint = &(pointInfos[current_point->num - 1]);
                pointInfos[next_point->num - 1].handle = points_dis_heap.push(pointInfos[next_point->num - 1]);      
                continue;          
            }

            if(connection_cost + pointInfos[current_point->num - 1].minDistance < pointInfos[next_point->num - 1].minDistance)
            {
                pointInfos[next_point->num - 1].minDistance = connection_cost + pointInfos[current_point->num - 1].minDistance;
                pointInfos[next_point->num - 1].prePoint = &(pointInfos[current_point->num - 1]);

                // update the minimum points
                points_dis_heap.decrease(pointInfos[next_point->num - 1].handle, pointInfos[next_point->num - 1]);
            }      

        }

        //find the nearest point that has not been reached
        current_point = cost_map->findPoint(points_dis_heap.top().label);
        points_dis_heap.pop();


    }while((pointInfos[current_point->num - 1].reachedBy != centralPoint) &&
           (pointInfos[current_point->num - 1].reachedBy != -1 )); /// XXX: it may need to change to make more than 2 start point condition works
}