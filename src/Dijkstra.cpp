/***********************************************************
 * \file      : Dijkstra.c
 * \brief     : This file contained a class for Dijkstra 
 *              algorithm
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/
#include "Dijkstra.h"
#include "iostream"

#include <algorithm>

Dijkstra::Dijkstra()
{
    path_point_num = 0;
}

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
        // TODO: add code to judge if there is no path



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


void Dijkstra::calculateShortestPath(uint16_t startPoint, uint16_t endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
    calculateShortestPath();
}