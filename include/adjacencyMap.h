/***********************************************************
 * @file      : adjacencyMap.h
 * @brief     : This file is used to represent a unidirecti-
 *              onal map that is used to store the data for 
 *              the shortest path algorithm, it contains a 
 *              matrix for shortest path algorithm
 *              
 * @copyright : Copyright Jiarun LIU
 * @author    : Jiarun LIU
 * @version   : V3.1
***********************************************************/
#ifndef __COSTMAP_H__
#define __COSTMAP_H__

#include <stdint.h>
#include <iostream>
#include <vector>
#include "mapGeometry.h"

class adjacencyMap
{
public:



    adjacencyMap();
    adjacencyMap(std::string nodefile, std::string elefile);
    adjacencyMap(adjacencyMap& copy);
    ~adjacencyMap();

    void clear();

    void readNodes(std::string nodefile);
    void readEles(std::string elefile);

    /**
     * @defgroup accuracy improvement functions
     * This is a group of function that used to improve the performance of the distance
     * @{
     */

    /**
     * @brief add the middle point to all the edges
    */
    void addMiddlePoints();


    /** @} */ // end of accuracy improvement functions



    ACCURACY getCost(pointCon_c* connection);
    POINT_NUM_MAX getPointNum();
    POINT_NUM_MAX getPathNum();
    std::vector<pointCon_c*> getAllConnections();
    std::vector<pointInfo_c*> getAllPoints();


    std::vector<pointCon_c*> getConnections(pointInfo_c* point);

    /// @brief find the points based on the num value of the point
    /// @param num_point : the num value for the points
    /// @return the target points address
    pointInfo_c* findPoint(POINT_NUM_MAX num_point);
    

    

    friend std::ostream& operator<< (std::ostream & out,const adjacencyMap &out_map);


protected:


private:


    std::vector<pointInfo_c*> points;
    std::vector<pointCon_c*> connections;
    std::vector<triangle_c*> triangles;   

};


#endif