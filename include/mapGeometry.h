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

#define SQRT_3 1.73205

// Change this if want to use double type accuracy
typedef float ACCURACY;
typedef float WEIGHT_ACCURACY;
typedef uint64_t POINT_NUM_MAX;
typedef uint32_t PATH_POINT_NUM_MAX;

class segment_c;
class point_c;
class triangle_c;


/// @brief Class defines all the proporities of a point
class point_c
{
public:
    POINT_NUM_MAX num;
    ACCURACY x;
    ACCURACY y;
    ACCURACY z;
    bool is_boundary;

    std::vector<segment_c*> connection;
    std::vector<triangle_c*> triangles;


    typedef enum{
        normal_point = 0,
        middle_point = 1,
        fermat_point = 2,
        circumcenter = 3,
        orthocenter  = 4,
        incenter     = 5
    }proporities_e;

    proporities_e proporities;

public:
    point_c(proporities_e porp = normal_point);
    point_c(ACCURACY x, ACCURACY y, ACCURACY z = 0, proporities_e prop = normal_point);
    ~point_c();

    /**
     * @brief add a connection to the point
     * @param adjacentPoint the connection point
     * @return the new point connection address
     * @retval nullptr: the connection is already exist
    */
    segment_c* addConnection(point_c* adjacentPoint);

    /**
     * @brief remove the connection to the point
     * @param adjacentPoint the connection class want to remove
     * @return if the connection removed successful
     * @retval true: remove successful
     * @retval false: connection may not exist
    */
    bool removeConnection(segment_c* connection);
    
    /// @brief decide if the point is already connected to the other one
    /// @param adjacentPoint  the next point
    /// @return 
    /// @retval true: are connected
    /// @retval false: not connected
    bool isConnected(point_c* adjacentPoint);

    /// @brief find the connection is exist or not
    /// @param adjacentPoint  the next point
    /// @return 
    /// @retval nullptr: no connection yet
    segment_c* findConnected(point_c* adjacentPoint);

};

/// @brief Class that discribe the connection of 2 points
class segment_c
{
public:    
    point_c* node1;
    point_c* node2;
    std::vector<triangle_c*> triangles;
    ACCURACY distance;

    /// Some special function points in the line
    point_c* middlePoint;


    typedef enum{
        normal_connection = 0,
        middle_connection = 1
    }proporities_e;

    proporities_e proporities;

    

public:
    segment_c(point_c* node_1 = nullptr, point_c* node_2 = nullptr);
    ~segment_c();


    double getDistance();
    double getDistance(point_c* node_1, point_c* node_2);

    point_c* getMiddlePoint();
    point_c* getMiddlePoint(point_c* node_1, point_c* node_2);

    /**
     * @brief this function is used to get the adjacent point of the current node
     * @return the adjacent node detail
    */
    point_c* getAnotherNode(point_c* current_node);
};







/// @brief class defines a triangle
class triangle_c
{
public:
    point_c* nodes[3];
    segment_c* connections[3];

    /// some special points in the triangle

    point_c* fermatPoint;
    point_c* middlePoint; /// or the centroid of the triangel
    point_c* circumCenter;
    point_c* orthoCenter;
    point_c* inCenter;

public:
    triangle_c(point_c* node_1, point_c* node_2, point_c* node_3);
    ~triangle_c();

    /// @brief used to initialize the connection of the triangle
    /// @note it can only be used once
    void initConnections();

    /// @brief used to get the another node in this triangle
    /// @param node_1 : the first node
    /// @param node_2 : the second node
    /// @return the address of the another node
    point_c* getAnotherNode(point_c* node_1, point_c* node_2);

    /// @brief used to get the another node in this triangle
    /// @param connection the edge in the triangle
    /// @return the address of the another node
    point_c* getAnotherNode(segment_c* connection);

    /// @brief used to get the adjacent triangle of the current triangle
    /// @param connection the shared edge with the adjacent triangle
    /// @return the pointer of the adjacent triangle
    /// @retval NULL: if the connection is not part of the triangle or there is no adjacent triangle
    triangle_c* getAdjacentTriangle(segment_c* connection);



    /// Algorithms used for triangle calculation

    /// @brief Used to get the area of the triangle
    /// @return the area of the triangle
    ACCURACY getArea();
    
    /// @brief calculate the cos value of the maximum angle
    /// @return the cos value of the maximum value for the triangle
    ACCURACY maximumAngleCos();


    /// @brief Used to get the fermat point of the triangle
    /// @return the address of the fermat point
    /// @retval nullptr: the fermat point does not exist
    /// @note no angle in the triangle is larger than 120 degree
    point_c* getFermatPoint();

    /// @brief Used to get the middle(centroid) point of the triangle
    /// @return the address of the middle point
    point_c* getMiddlePoint();

    /// @brief Used to get the circumcenter of the triangle
    /// @return the address of the circumcenter
    /// @note no angle in the triangle is larger than 90 degree (otherwise the point is out side the triangle)
    point_c* getCircumCenter();

    /// @brief Used to get the orthocenter of the triangle
    /// @return the address of the orthocenter
    /// @note no angle in the triangle is larger than 90 degree (otherwise the point is out side the triangle)
    point_c* getOrthoCenter();

    /// @brief Used to get the incenter of the triangle
    /// @return the address of the incenter
    point_c* getInCenter();

private:

    /// @brief This algorithm is used to calculate the tc used to calculate the Fermat point
    /// @return tc
    ACCURACY tc_Fermat(ACCURACY area, ACCURACY s, ACCURACY t, ACCURACY u);




};


#endif