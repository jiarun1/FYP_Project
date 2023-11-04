/***********************************************************
 * @file      : costMap.h
 * @brief     : This file is used to represent a unidirecti-
 *              onal map that is used to store the data for 
 *              the shortest path algorithm, it contains a 
 *              matrix for shortest path algorithm
 *              
 * @copyright : Copyright Jiarun LIU
 * @author    : Jiarun LIU
 * @version   : 
***********************************************************/
#ifndef __COSTMAP_H__
#define __COSTMAP_H__

#include <stdint.h>
#include <iostream>

class costMap
{
public:
    // Change this if want to use double type accuracy
    typedef float DISTANCE_ACCURACY;
    typedef float WEIGHT_ACCURACY;


public:



    costMap();
    costMap(uint32_t point_num);
    costMap(costMap& copy);
    ~costMap();

    void setCost(int start_point, int end_point, DISTANCE_ACCURACY dis, WEIGHT_ACCURACY weight = 1.);
    DISTANCE_ACCURACY getCost(int start_point, int end_point);

    uint32_t getPointNum(void);






    friend std::ostream& operator<< (std::ostream & out,const costMap &out_map);
    //TODO: add the code for cin


protected:


private:
    /// @brief this parameter used to store the cost(distance*weight) between 2 point, in matrix format
    ///
    /// @note  the -1 is represent the line is not connected
    DISTANCE_ACCURACY** cost; ///< size: point_num*point_num

    /// @brief this parameter used to store the number of points in this map
    uint32_t point_num;
    

};
#endif