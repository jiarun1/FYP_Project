/***********************************************************
 * @file      : adjacencyMap.h
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

#define ADJACENT_NO_PATH (-1)

class adjacencyMap
{
public:
    // Change this if want to use double type accuracy
    typedef float DISTANCE_ACCURACY;
    typedef float WEIGHT_ACCURACY;

    typedef struct
    {
        float x;
        float y;
        float z;
    }pointLocation;
    


public:



    adjacencyMap();
    adjacencyMap(uint32_t point_num);
    adjacencyMap(adjacencyMap& copy);
    ~adjacencyMap();


    void clear();

    void setCost(int start_point, int end_point, DISTANCE_ACCURACY dis, WEIGHT_ACCURACY weight = 1.);
    DISTANCE_ACCURACY getCost(int start_point, int end_point);

    void setPointNum(uint32_t point_num);
    uint32_t getPointNum(void);

    void setPointPosition(uint32_t point_label ,float x, float y, float z);
    pointLocation* getPointPosition(uint32_t point_label);

    /**
     * @brief: get the number of path that is available to go through
     * 
     * @note: it consider as directional path, one path would be consider twice
    */
    uint32_t getPathNum(void);



    friend std::ostream& operator<< (std::ostream & out,const adjacencyMap &out_map);
    //TODO: add the code for cin


protected:


private:
    /// @brief this parameter used to store the cost(distance*weight) between 2 point, in matrix format
    ///
    /// @note  the -1 is represent the line is not connected
    DISTANCE_ACCURACY** cost; ///< size: pointNum*pointNum

    /// @brief this parameter used to store the number of points in this map
    uint32_t pointNum;

    /// @brief this parameter 
    pointLocation* pointLoc;
    

};


static const adjacencyMap::DISTANCE_ACCURACY NO_CONNECTION = -1.0;


#endif