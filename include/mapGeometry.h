/***********************************************************
 * @file      : mapGeometry.h
 * @brief     : 
 *              
 * @copyright : Copyright Jiarun LIU
 * @author    : Jiarun LIU
 * @version   : V3.1
***********************************************************/
#ifndef __MAPGEOMETRY_H__
#define __MAPGEOMETRY_H__

#include <vector>
#include <float.h>
#include <stdint.h>

// Change this if want to use double type accuracy
typedef float ACCURACY;
typedef float WEIGHT_ACCURACY;
typedef uint64_t POINT_NUM_MAX;
typedef uint32_t PATH_POINT_NUM_MAX;

class pointInfo_c;

class pointCon_c
{
public:    
    pointInfo_c* node1;
    pointInfo_c* node2;
    ACCURACY distance;
public:
    pointCon_c(pointInfo_c* node_1 = nullptr, pointInfo_c* node_2 = nullptr);
    ~pointCon_c();

    double calDistance(pointInfo_c* node_1, pointInfo_c* node_2);

    /**
     * @brief this function is used to get the adjacent point of the current node
     * @return the adjacent node detail
    */
    pointInfo_c* getAnotherNode(pointInfo_c* current_node);
};



class pointInfo_c
{
public:
    POINT_NUM_MAX num;
    ACCURACY x;
    ACCURACY y;
    ACCURACY z;
    bool is_boundary;

    std::vector<pointCon_c*> connection;
public:
    pointInfo_c();
    ~pointInfo_c();

    /**
     * @brief add a connection to the point
     * @param adjacentPoint the connection point
     * @return the new point connection address
     * @retval nullptr: the connection is already exist
    */
    pointCon_c* addConnection(pointInfo_c* adjacentPoint);
    
    /// @brief decide if the point is already connected to the other one
    /// @param adjacentPoint  the next point
    /// @return 
    /// @retval true: are connected
    /// @retval false: not connected
    bool isConnected(pointInfo_c* adjacentPoint);
};



#endif