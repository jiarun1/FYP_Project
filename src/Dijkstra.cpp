/***********************************************************
 * \file      : Dijkstra.c
 * \brief     : This file contained a class for Dijkstra 
 *              algorithm
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V1.1
***********************************************************/
#include "Dijkstra.h"
#include "iostream"

#include <algorithm>

#define VERSION_V1_1


Dijkstra::Dijkstra()
{
    path_point_num = 0;
}

#ifdef VERSION_V1_1
void Dijkstra::calculateShortestPath()
{
    uint16_t point_num = cost_map->getPointNum();


    // this parameter used to stored the minimin distance to the point
    //  - if the value is -1 means the point is not reached yet
    adjacencyMap::DISTANCE_ACCURACY points_info[point_num];
    std::fill_n(points_info,point_num,-1); //< initialize as -1


    // this parameter used to stored if the points is close to the reached point and able to calculate
    //  - if the value is -1 means the point is not reached yet
    bool points_available[point_num] = {false};
    bool points_reached[point_num] = {false};
    
    
    // record the previous point for this point when minimum
    // - if the value is -1 means the point is not reached yet
    int32_t    previous_point[point_num];
    std::fill_n(previous_point,point_num,-1); //< initialize as -1

    // initialize the start point
    points_info[start_point] = 0;

    uint16_t current_point = start_point;

    // std::cout << *cost_map << std::endl;
   
    do{
        points_reached[current_point] = true;
        points_available[current_point] = false;

        // based on the previous point, update all the point distance near the current one
        for(int i = 0; i < point_num; i++)
        {
            adjacencyMap::DISTANCE_ACCURACY cost = cost_map->getCost(current_point, i);
            if(cost == -1)
            {
                continue;
            }

            if(points_reached[i] == true)
            {
                continue;
            }

            if(points_info[i] == -1)
            {
                points_info[i] = cost + points_info[current_point];
                previous_point[i] = current_point;
                points_available[i] = true;
            }

            if(cost + points_info[current_point] < points_info[i])
            {
                points_info[i] = cost + points_info[current_point];
                previous_point[i] = current_point;
                points_available[i] = true;
            }            
        }

        // find the nearest point that has not been reached
        adjacencyMap::DISTANCE_ACCURACY min_cost = -1;
        for(int i = 0; i < point_num; i++)
        {
            if(points_available[i] == false)
            {
                continue;
            }

            if(min_cost == -1)
            {
                min_cost = points_info[i];
                current_point = i;
                continue;
            }

            if(points_info[i] < min_cost)
            {
                min_cost = points_info[i];
                current_point = i;
                continue;
            }
        }


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
    while (previous_point[point_tmp] != start_point)
    {
        passed_point_num ++;
        point_tmp = previous_point[point_tmp];
    }
    
    // store the path to the result
    path_point_num = passed_point_num;
    path_cost = points_info[end_point];
    path = new uint16_t[path_point_num];

    path[path_point_num-1] = end_point;
    for(int i = path_point_num - 2; i > 0; i--)
    {
        path[i] = previous_point[path[i+1]];
    }
    path[0] = start_point;

}
#endif

#ifdef VERSION_V1_0
/// the version 1 code for dijkstra algorithm
void Dijkstra::calculateShortestPath()
{
    uint16_t point_num = cost_map->getPointNum();


    // this parameter used to stored the minimin distance to the point
    //  - if the value is -1 means the point is not reached yet
    adjacencyMap::DISTANCE_ACCURACY points_info[point_num];
    std::fill_n(points_info,point_num,-1); //< initialize as -1
    
    // record the previous point for this point when minimum
    // - if the value is -1 means the point is not reached yet
    int32_t    previous_point[point_num];
    std::fill_n(previous_point,point_num,-1); //< initialize as -1

    // initialize the start point
    points_info[start_point] = 0;
   
    // std::cout << *cost_map << std::endl;

    while(points_info[end_point] == -1)
    {
        adjacencyMap::DISTANCE_ACCURACY min_distance = -1;
        uint16_t current_point, next_point;

        // this loop find the points that has reached
        for(int i = 0; i < point_num; i++)
        {

            // if not reached this point, continue
            if(points_info[i] == -1)
            {
                continue;
            }

            // this loop find if the points is not reached yet
            for(int j = 0; j < point_num; j++)
            {

                if(points_info[j] != -1){
                    // if reached this point, continue
                    continue;
                }

                // some thing went wrong
                if(i == j){
                    std::cerr << "The Algorithm Facing Deadly bugs" << std::endl;
                    exit(2);
                }

                // skip this if there is no connection between 2 point
                if(cost_map->getCost(i,j) == -1)
                {
                    continue;
                }

                // get the distance from startpoint to (point j) through (point i)
                adjacencyMap::DISTANCE_ACCURACY cur_distance = cost_map->getCost(i,j) + points_info[i]; 

                if(min_distance == -1){
                    // this means this is the first path the algorithm find
                    min_distance = cur_distance;
                    current_point = i;
                    next_point = j;
                } else {
                    // judge if the distance need to update
                    if(min_distance > cur_distance){
                        min_distance = cur_distance;
                        current_point = i;
                        next_point = j;
                    }
                }
            }
        }

        points_info[next_point] = min_distance;
        previous_point[next_point] = current_point;
    }  

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
    while (previous_point[point_tmp] != start_point)
    {
        passed_point_num ++;
        point_tmp = previous_point[point_tmp];
    }
    
    // store the path to the result
    path_point_num = passed_point_num;
    path_cost = points_info[end_point];
    path = new uint16_t[path_point_num];

    path[path_point_num-1] = end_point;
    for(int i = path_point_num - 2; i > 0; i--)
    {
        path[i] = previous_point[path[i+1]];
    }
    path[0] = start_point;

}
#endif

void Dijkstra::calculateShortestPath(uint16_t startPoint, uint16_t endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
    calculateShortestPath();
}