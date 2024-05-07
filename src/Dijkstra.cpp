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
    // pointInfos(nullptr),
    shortestPathAlgorithm()
{

}

Dijkstra::~Dijkstra()
{
    
    // if(pointInfos != nullptr)
    // {
    //     delete[] pointInfos;
    // }
}


void Dijkstra::setAdjacencyMap(adjacencyMap* map)
{
    cost_map = map;
    // pointInfos = new pointInfoContainer[map->getPointNum()];

    // give all the point the label of it
    // for(int i = 0; i < map->getPointNum(); i++)
    // {
    //     pointInfos[i].label = i + 1;
    // }
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


void Dijkstra::calculateShortestPath_Bidirectional(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
    calculateShortestPath_Bidirectional();
}


void Dijkstra::calculateShortestPath()
{
    std::cout << "Standard Dijkstra Algorithm is running" << std::endl;

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


void Dijkstra::calculateShortestPath_Bidirectional()
{
    std::cout << "Bidirectional Dijkstra Algorithm is running" << std::endl;

    uint8_t forward = 0;
    uint8_t backward = 1;

    POINT_NUM_MAX point_num = cost_map->getPointNum();

    pointInfoContainer* points_info = new pointInfoContainer[point_num];

    for(int i = 0; i < point_num; i++)
    {
        points_info[i].label = i + 1;
        points_info[i].init(2);
    }

    boost::heap::fibonacci_heap<pointInfoHeap> points_dis_heap[2];

    point_c* current_point[2];
    // point_c *forward_reach_point, *backward_reach_point;
    point_c* reached_point = nullptr;

    current_point[forward] = cost_map->findPoint(start_point);
    current_point[backward] = cost_map->findPoint(end_point);


    // initialize the start point
    points_info[current_point[forward]->num - 1].heap_data[forward].minDistance = 0;
    points_info[current_point[backward]->num - 1].heap_data[backward].minDistance = 0;

    do{
        
        /*************************** Forward Dijkstra  *********************************/
        points_info[current_point[forward]->num - 1].reachedBy = start_point;

        // based on the previous point, update all the point distance near the current one
        auto current_connection_path_forward = cost_map->getConnections(current_point[forward]);

        for(int i = 0; i < current_connection_path_forward.size(); i++)
        {
            auto next_point = current_connection_path_forward[i]->getAnotherNode(current_point[forward]);
            auto connection_cost = current_connection_path_forward[i]->distance;

            // if the point is reached, goto the next point
            if(points_info[next_point->num - 1].reachedBy == start_point){
                continue;
            }

            if((points_info[next_point->num - 1].heap_data[forward].minDistance) == -1)
            {
                points_info[next_point->num - 1].heap_data[forward].minDistance = connection_cost + points_info[current_point[forward]->num - 1].heap_data[forward].minDistance;
                points_info[next_point->num - 1].heap_data[forward].prePoint = &(points_info[current_point[forward]->num - 1].heap_data[forward]);
                points_info[next_point->num - 1].heap_data[forward].handle = points_dis_heap[forward].push(points_info[next_point->num - 1].heap_data[forward]);      
                continue;          
            }

            if(connection_cost + points_info[current_point[forward]->num - 1].heap_data[forward].minDistance < points_info[next_point->num - 1].heap_data[forward].minDistance)
            {
                points_info[next_point->num - 1].heap_data[forward].minDistance = connection_cost + points_info[current_point[forward]->num - 1].heap_data[forward].minDistance;
                points_info[next_point->num - 1].heap_data[forward].prePoint = &(points_info[current_point[forward]->num - 1]).heap_data[forward];

                // update the minimum points
                points_dis_heap[forward].decrease(points_info[next_point->num - 1].heap_data[forward].handle, points_info[next_point->num - 1].heap_data[forward]);
            }      

        }

        //find the nearest point that has not been reached
        auto next_point_forward = cost_map->findPoint(points_dis_heap[forward].top().parent->label);

        if(points_info[next_point_forward->num - 1].reachedBy == end_point) {
            reached_point = next_point_forward;
            // backward_reach_point = next_point_forward;
            // forward_reach_point = cost_map->findPoint(points_info[next_point_forward->num - 1].prePoint[forward]->label);
            // path_cost = points_info[next_point_forward->num - 1].minDistance[forward] + points_info[next_point_forward->num - 1].minDistance[backward];
            break;
        } else {
            current_point[forward] = next_point_forward;
        }

        points_dis_heap[forward].pop();


        /*************************** Backward Dijkstra  *********************************/
        points_info[current_point[backward]->num - 1].reachedBy = end_point;

        // based on the previous point, update all the point distance near the current one
        auto current_connection_path_backward = cost_map->getConnections(current_point[backward]);

        for(int i = 0; i < current_connection_path_backward.size(); i++)
        {
            auto next_point = current_connection_path_backward[i]->getAnotherNode(current_point[backward]);
            auto connection_cost = current_connection_path_backward[i]->distance;

            // if the point is reached, goto the next point
            if(points_info[next_point->num - 1].reachedBy == end_point){
                continue;
            }

            if((points_info[next_point->num - 1].heap_data[backward].minDistance) == -1)
            {
                points_info[next_point->num - 1].heap_data[backward].minDistance = connection_cost + points_info[current_point[backward]->num - 1].heap_data[backward].minDistance;
                points_info[next_point->num - 1].heap_data[backward].prePoint = &(points_info[current_point[backward]->num - 1].heap_data[backward]);
                points_info[next_point->num - 1].heap_data[backward].handle = points_dis_heap[backward].push(points_info[next_point->num - 1].heap_data[backward]);      
                continue;          
            }

            if(connection_cost + points_info[current_point[backward]->num - 1].heap_data[backward].minDistance < points_info[next_point->num - 1].heap_data[backward].minDistance)
            {
                points_info[next_point->num - 1].heap_data[backward].minDistance = connection_cost + points_info[current_point[backward]->num - 1].heap_data[backward].minDistance;
                points_info[next_point->num - 1].heap_data[backward].prePoint = &(points_info[current_point[backward]->num - 1]).heap_data[backward];

                // update the minimum points
                points_dis_heap[backward].decrease(points_info[next_point->num - 1].heap_data[backward].handle, points_info[next_point->num - 1].heap_data[backward]);
            }      

        }

        //find the nearest point that has not been reached
        auto next_point_backward = cost_map->findPoint(points_dis_heap[backward].top().parent->label);

        if(points_info[next_point_backward->num - 1].reachedBy == start_point) {
            reached_point = next_point_backward;
            break;
        } else {
            current_point[backward] = next_point_backward;
        }

        points_dis_heap[backward].pop();

    }while(1);

    
    double min_path_length = -1;
    // for(int i = 0; i < point_num; i++)
    // {
    //     if(points_info[i].heap_data[forward].minDistance == -1)
    //     {
    //         continue;
    //     }
    //     if(points_info[i].heap_data[backward].minDistance == -1)
    //     {
    //         continue;
    //     }

    //     if(min_path_length == -1)
    //     {
    //         min_path_length = points_info[i].heap_data[forward].minDistance + points_info[i].heap_data[backward].minDistance;
    //         reached_point = cost_map->findPoint(points_info[i].label);
    //     }
        
    //     if(points_info[i].heap_data[forward].minDistance + points_info[i].heap_data[backward].minDistance < min_path_length)
    //     {
    //         min_path_length = points_info[i].heap_data[forward].minDistance + points_info[i].heap_data[backward].minDistance;
    //         reached_point = cost_map->findPoint(points_info[i].label);
    //     }
    // }



    for(int i = 0; i < points_dis_heap[forward].size(); i++)
    {
        auto reach_point_tmp = points_dis_heap[forward].top().parent;
        points_dis_heap[forward].pop();

        if(reach_point_tmp->heap_data[forward].minDistance == -1)
        {
            continue;
        }

        if(reach_point_tmp->heap_data[backward].minDistance == -1)
        {
            continue;
        }

        auto min_length_tmp = reach_point_tmp->heap_data[forward].minDistance + reach_point_tmp->heap_data[backward].minDistance;

        if(min_path_length == -1)
        {
            min_path_length = min_length_tmp;
            reached_point = cost_map->findPoint(reach_point_tmp->label);
        }
        
        if(min_length_tmp < min_path_length)
        {
            min_path_length = min_length_tmp;
            reached_point = cost_map->findPoint(reach_point_tmp->label);
        }
    }

    for(int i = 0; i < points_dis_heap[backward].size(); i++)
    {
        auto reach_point_tmp = points_dis_heap[backward].top().parent;
        points_dis_heap[backward].pop();

        if(reach_point_tmp->heap_data[forward].minDistance == -1)
        {
            continue;
        }

        if(reach_point_tmp->heap_data[backward].minDistance == -1)
        {
            continue;
        }

        auto min_length_tmp = reach_point_tmp->heap_data[forward].minDistance + reach_point_tmp->heap_data[backward].minDistance;

        if(min_path_length == -1)
        {
            min_path_length = min_length_tmp;
            reached_point = cost_map->findPoint(reach_point_tmp->label);
        }
        
        if(min_length_tmp < min_path_length)
        {
            min_path_length = min_length_tmp;
            reached_point = cost_map->findPoint(reach_point_tmp->label);
        }
    }

    path_cost = min_path_length;


    /// update the path information

    // delete the previous pass data
    if(path_point_num != 0)
    {   
        delete[] path;
    }

    /// forward part
    // used to count the length of the path
    PATH_POINT_NUM_MAX passed_point_num[2];  //< count the start point and the end point
    passed_point_num[0] = 2;
    passed_point_num[1] = 2;

    // POINT_NUM_MAX point_tmp = forward_reach_point->num;
    POINT_NUM_MAX point_tmp = reached_point->num;
    // contineous find the previous point until reach the start point
    if(points_info[point_tmp - 1].label != start_point)
    {
    // contineous find the previous point until reach the start point
        while (points_info[point_tmp - 1].heap_data[forward].prePoint->parent->label != start_point)
        {
            passed_point_num[forward] ++;
            point_tmp = points_info[point_tmp - 1].heap_data[forward].prePoint->parent->label;
        }
    }

    // point_tmp = forward_reach_point->num;
    point_tmp = reached_point->num;
    if(points_info[point_tmp - 1].label != end_point)
    {
        while (points_info[point_tmp - 1].heap_data[backward].prePoint->parent->label != end_point)
        {
            passed_point_num[backward] ++;
            point_tmp = points_info[point_tmp - 1].heap_data[backward].prePoint->parent->label;
        }
    }

    path_point_num = passed_point_num[forward] + passed_point_num[backward] - 1;

    // path_cost = points_info[end_point - 1].minDistance[forward];
    path = new PATH_POINT_NUM_MAX[path_point_num];

    path[path_point_num-1] = end_point;
    path[0] = start_point;
    // path[passed_point_num[forward] - 1] = forward_reach_point->num;
    // path[passed_point_num[forward]] = backward_reach_point->num;]
    path[passed_point_num[forward] - 1] = reached_point->num;


    // for(int i = passed_point_num[forward] - 2; i > 0; i--)
    // {
    //     path[i] = points_info[path[i+1]-1].prePoint[forward]->label;
    // }
    // for(int i = passed_point_num[forward] + 1; i < path_point_num - 1; i++)
    // {
    //     path[i] = points_info[path[i-1]-1].prePoint[backward]->label;
    // }

    for(int i = passed_point_num[forward] - 2; i > 0; i--)
    {
        path[i] = points_info[path[i+1]-1].heap_data[forward].prePoint->parent->label;
    }
    for(int i = passed_point_num[forward]; i < path_point_num - 1; i++)
    {
        path[i] = points_info[path[i-1]-1].heap_data[backward].prePoint->parent->label;
    }

    for(int i = 0; i < point_num; i++)
    {
        points_info[i].release();
    }
}

void Dijkstra::calculateShortestPathInParallel(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
    calculateShortestPathInParallel();
}

void Dijkstra::calculateShortestPathInParallel()
{
    
    std::cout << "Parallel Bidirectional Dijkstra Algorithm is running" << std::endl;

    uint8_t forward = 0;
    uint8_t backward = 1;

    POINT_NUM_MAX point_num = cost_map->getPointNum();

    pointInfoContainer* points_info = new pointInfoContainer[point_num];

    for(int i = 0; i < point_num; i++)
    {
        points_info[i].label = i + 1;
        points_info[i].init(2);
    }

    // point_c *forward_reach_point, *backward_reach_point;
    point_c* reached_point = nullptr;

    points_info[cost_map->findPoint(start_point)->num - 1].reachedBy = start_point;
    points_info[cost_map->findPoint(end_point)->num - 1].reachedBy = end_point;

    std::thread startThread(&Dijkstra::findPathFrom, this, points_info, reached_point, start_point, 0);
    std::thread endThread(&Dijkstra::findPathFrom, this,  points_info, reached_point, end_point, 1);

    startThread.join();
    endThread.join();

    if((startThread.joinable()) || (endThread.joinable()))
    {
        std::cout << "thread is not correct finished" << std::endl;
    }

    double min_path_length = -1;
    for(int i = 0; i < point_num; i++)
    {
        if(points_info[i].heap_data[forward].minDistance == -1)
        {
            continue;
        }
        if(points_info[i].heap_data[backward].minDistance == -1)
        {
            continue;
        }

        if(min_path_length == -1)
        {
            min_path_length = points_info[i].heap_data[forward].minDistance + points_info[i].heap_data[backward].minDistance;
            reached_point = cost_map->findPoint(points_info[i].label);
        }
        
        if(points_info[i].heap_data[forward].minDistance + points_info[i].heap_data[backward].minDistance < min_path_length)
        {
            min_path_length = points_info[i].heap_data[forward].minDistance + points_info[i].heap_data[backward].minDistance;
            reached_point = cost_map->findPoint(points_info[i].label);
        }
    }

    
    path_cost = min_path_length;

    // std::cout << "stop target reach" << std::endl;


    // delete the previous pass data
    if(path_point_num != 0)
    {   
        delete[] path;
    }

    /// forward part
    // used to count the length of the path
    PATH_POINT_NUM_MAX passed_point_num[2];  //< count the start point and the end point
    passed_point_num[0] = 2;
    passed_point_num[1] = 2;

    // POINT_NUM_MAX point_tmp = forward_reach_point->num;
    POINT_NUM_MAX point_tmp = reached_point->num;
    if(points_info[point_tmp - 1].label != start_point)
    {
    // contineous find the previous point until reach the start point
        while (points_info[point_tmp - 1].heap_data[forward].prePoint->parent->label != start_point)
        {
            passed_point_num[forward] ++;
            point_tmp = points_info[point_tmp - 1].heap_data[forward].prePoint->parent->label;
        }
    }

    // point_tmp = forward_reach_point->num;
    point_tmp = reached_point->num;
    if(points_info[point_tmp - 1].label != end_point)
    {
        while (points_info[point_tmp - 1].heap_data[backward].prePoint->parent->label != end_point)
        {
            passed_point_num[backward] ++;
            point_tmp = points_info[point_tmp - 1].heap_data[backward].prePoint->parent->label;
        }
    }

    path_point_num = passed_point_num[forward] + passed_point_num[backward] - 1;

    // std::cout << "path point num: " << passed_point_num[forward] << " , " << passed_point_num[backward] << std::endl;

    // path_cost = points_info[end_point - 1].minDistance[forward];
    path = new PATH_POINT_NUM_MAX[path_point_num];

    path[path_point_num-1] = end_point;
    path[0] = start_point;

    path[passed_point_num[forward] - 1] = reached_point->num;


    for(int i = passed_point_num[forward] - 2; i > 0; i--)
    {
        path[i] = points_info[path[i+1]-1].heap_data[forward].prePoint->parent->label;
    }
    for(int i = passed_point_num[forward]; i < path_point_num - 1; i++)
    {
        path[i] = points_info[path[i-1]-1].heap_data[backward].prePoint->parent->label;
    }

    for(int i = 0; i < point_num; i++)
    {
        points_info[i].release();
    }

    delete[] points_info;
}


void Dijkstra::findPathFrom(pointInfoContainer* points_info, point_c* reached_point, POINT_NUM_MAX centralPoint, int thread_label)
{
    point_c* current_point = cost_map->findPoint(centralPoint);

    points_info[current_point->num - 1].heap_data[thread_label].minDistance = 0;

    boost::heap::fibonacci_heap<pointInfoHeap> points_dis_heap;


    while (1)
    {
        // std::cout << "thread " << thread_label << " reached to: " << current_point->num << std::endl;

        points_info[current_point->num - 1].reachedBy = centralPoint;

        // based on the previous point, update all the point distance near the current one
        auto current_connection_path_forward = cost_map->getConnections(current_point);

        for(int i = 0; i < current_connection_path_forward.size(); i++)
        {
            auto next_point = current_connection_path_forward[i]->getAnotherNode(current_point);
            auto connection_cost = current_connection_path_forward[i]->distance;

            // if the point is reached, goto the next point
            if(points_info[next_point->num - 1].reachedBy == start_point){
                continue;
            }

            if((points_info[next_point->num - 1].heap_data[thread_label].minDistance) == -1)
            {
                points_info[next_point->num - 1].heap_data[thread_label].minDistance = connection_cost + points_info[current_point->num - 1].heap_data[thread_label].minDistance;
                points_info[next_point->num - 1].heap_data[thread_label].prePoint = &(points_info[current_point->num - 1].heap_data[thread_label]);
                points_info[next_point->num - 1].heap_data[thread_label].handle = points_dis_heap.push(points_info[next_point->num - 1].heap_data[thread_label]);      
                continue;          
            }

            if(connection_cost + points_info[current_point->num - 1].heap_data[thread_label].minDistance < points_info[next_point->num - 1].heap_data[thread_label].minDistance)
            {
                points_info[next_point->num - 1].heap_data[thread_label].minDistance = connection_cost + points_info[current_point->num - 1].heap_data[thread_label].minDistance;
                points_info[next_point->num - 1].heap_data[thread_label].prePoint = &(points_info[current_point->num - 1]).heap_data[thread_label];

                // update the minimum points
                points_dis_heap.decrease(points_info[next_point->num - 1].heap_data[thread_label].handle, points_info[next_point->num - 1].heap_data[thread_label]);
            }      

        }


        if (points_dis_heap.empty()) {
            std::cout << "Heap is empty, no more points to process." << std::endl;
            return;
        }

        //find the nearest point that has not been reached
        auto next_point_forward = cost_map->findPoint(points_dis_heap.top().parent->label);

        if((points_info[next_point_forward->num - 1].reachedBy != centralPoint)  && (points_info[next_point_forward->num - 1].reachedBy != 0) ){
            reached_point = next_point_forward;
            // std::cout << "thread " << thread_label << " finished by visit" <<std::endl;
            return;
        } else {
            current_point = next_point_forward;
        }
        
        // if other thread are stopped.
        if(reached_point != nullptr)
        {
            // std::cout << "thread " << thread_label << " finished" <<std::endl;
            return;
        }
        points_dis_heap.pop();
    }
    

    // boost::heap::fibonacci_heap<pointInfoHeap> points_dis_heap;

    // point_c* current_point = cost_map->findPoint(start_point);

    // // initialize the start point
    // pointInfos[current_point->num - 1].minDistance = 0;
    // pointInfos[current_point->num - 1].reachedBy = centralPoint;

   
    // do{
    //     pointInfos[current_point->num - 1].reachedBy = centralPoint;

    //     // based on the previous point, update all the point distance near the current one
    //     auto current_connection_path = cost_map->getConnections(current_point);

    //     for(int i = 0; i < current_connection_path.size(); i++)
    //     {
    //         auto next_point = current_connection_path[i]->getAnotherNode(current_point);
    //         auto connection_cost = current_connection_path[i]->distance;

    //         // if the point is reached, goto the next point
    //         if(pointInfos[next_point->num - 1].reachedBy == centralPoint){
    //             continue;
    //         }

    //         if(pointInfos[next_point->num - 1].minDistance == -1)
    //         {
    //             pointInfos[next_point->num - 1].minDistance = connection_cost + pointInfos[current_point->num - 1].minDistance;
    //             pointInfos[next_point->num - 1].prePoint = &(pointInfos[current_point->num - 1]);
    //             pointInfos[next_point->num - 1].handle = points_dis_heap.push(pointInfos[next_point->num - 1]);      
    //             continue;          
    //         }

    //         if(connection_cost + pointInfos[current_point->num - 1].minDistance < pointInfos[next_point->num - 1].minDistance)
    //         {
    //             pointInfos[next_point->num - 1].minDistance = connection_cost + pointInfos[current_point->num - 1].minDistance;
    //             pointInfos[next_point->num - 1].prePoint = &(pointInfos[current_point->num - 1]);

    //             // update the minimum points
    //             points_dis_heap.decrease(pointInfos[next_point->num - 1].handle, pointInfos[next_point->num - 1]);
    //         }      

    //     }

    //     //find the nearest point that has not been reached
    //     current_point = cost_map->findPoint(points_dis_heap.top().label);
    //     points_dis_heap.pop();


    // }while((pointInfos[current_point->num - 1].reachedBy != centralPoint) &&
    //        (pointInfos[current_point->num - 1].reachedBy != -1 )); /// XXX: it may need to change to make more than 2 start point condition works
}