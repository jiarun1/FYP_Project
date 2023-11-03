/***********************************************************
 * @file      : unidirectionalMap.h
 * @brief     : This file is used to represent a unidirecti-
 *              onal map that is used to store the data for 
 *              the shortest path algorithm, it contains a 
 *              matrix for shortest path algorithm
 *              
 * @copyright : Copyright Jiarun LIU
 * @author    : Jiarun LIU
 * @version   : 
***********************************************************/
#ifndef __UNIDIRECTIONALMAP_H__
#define __UNIDIRECTIONALMAP_H__

#include <stdint.h>
#include <iostream>

class unidirectionalMap
{
public:
    // Change this if want to use double type accuracy
    typedef float MAP_ACCURACY;


public:



    unidirectionalMap();
    unidirectionalMap(uint32_t point_num);
    unidirectionalMap(unidirectionalMap& copy);
    ~unidirectionalMap();

    void setDistance(int start_point, int end_point, MAP_ACCURACY distance);







    friend std::ostream& operator<< (std::ostream & out,const unidirectionalMap &out_map);
    //TODO: add the code for cin


protected:


private:
    /// @brief this parameter used to store the distance between 2 point, in matrix format
    ///
    /// @note  the -1 is represent the line is not connected
    MAP_ACCURACY** distance; ///< size: pointNum*pointNum

    /// @brief this parameter used to store the number of points in this map
    uint32_t pointNum;
    

};
#endif