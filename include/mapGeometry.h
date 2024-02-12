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

class pointCon_c;
class pointInfo_c;
class triangle_c;


/// @brief Class that discribe the connection of 2 points
class pointCon_c
{
public:    
    pointInfo_c* node1;
    pointInfo_c* node2;
    std::vector<triangle_c*> triangles;
    ACCURACY distance;

    /// Some special function points in the line
    pointInfo_c* middlePoint;


    typedef enum{
        normal_connection = 0,
        middle_connection = 1
    }proporities_e;

    proporities_e proporities;

    

public:
    pointCon_c(pointInfo_c* node_1 = nullptr, pointInfo_c* node_2 = nullptr);
    ~pointCon_c();


    double calDistance();
    double calDistance(pointInfo_c* node_1, pointInfo_c* node_2);

    pointInfo_c* getMiddlePoint();
    pointInfo_c* getMiddlePoint(pointInfo_c* node_1, pointInfo_c* node_2);

    /**
     * @brief this function is used to get the adjacent point of the current node
     * @return the adjacent node detail
    */
    pointInfo_c* getAnotherNode(pointInfo_c* current_node);
};





/// @brief Class defines all the proporities of a point
class pointInfo_c
{
public:
    POINT_NUM_MAX num;
    ACCURACY x;
    ACCURACY y;
    ACCURACY z;
    bool is_boundary;

    std::vector<pointCon_c*> connection;
    std::vector<triangle_c*> triangles;


    typedef enum{
        normal_point = 0,
        middle_point = 1
    }proporities_e;

    proporities_e proporities;

public:
    pointInfo_c(proporities_e porp = normal_point);
    ~pointInfo_c();

    /**
     * @brief add a connection to the point
     * @param adjacentPoint the connection point
     * @return the new point connection address
     * @retval nullptr: the connection is already exist
    */
    pointCon_c* addConnection(pointInfo_c* adjacentPoint);

    /**
     * @brief remove the connection to the point
     * @param adjacentPoint the connection class want to remove
     * @return if the connection removed successful
     * @retval true: remove successful
     * @retval false: connection may not exist
    */
    bool removeConnection(pointCon_c* connection);
    
    /// @brief decide if the point is already connected to the other one
    /// @param adjacentPoint  the next point
    /// @return 
    /// @retval true: are connected
    /// @retval false: not connected
    bool isConnected(pointInfo_c* adjacentPoint);

    /// @brief find the connection is exist or not
    /// @param adjacentPoint  the next point
    /// @return 
    /// @retval nullptr: no connection yet
    pointCon_c* findConnected(pointInfo_c* adjacentPoint);
};


/// @brief class defines a triangle
class triangle_c
{
public:
    pointInfo_c* nodes[3];
    pointCon_c* connections[3];


public:
    triangle_c(pointInfo_c* node_1, pointInfo_c* node_2, pointInfo_c* node_3);
    ~triangle_c();

    /// @brief used to initialize the connection of the triangle
    /// @note it can only be used once
    void initConnections();

    /// @brief used to get the another node in this triangle
    /// @param node_1 : the first node
    /// @param node_2 : the second node
    /// @return the address of the another node
    pointInfo_c* getAnotherNode(pointInfo_c* node_1, pointInfo_c* node_2);

    /// @brief used to get the another node in this triangle
    /// @param connection the edge in the triangle
    /// @return the address of the another node
    pointInfo_c* getAnotherNode(pointCon_c* connection);
};


#endif