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

    ACCURACY getCost(pointCon_c* connection);
    uint32_t getPointNum();
    uint32_t getPathNum();
    std::vector<pointCon_c*> getAllConnections();
    std::vector<pointInfo_c*> getAllPoints();


    std::vector<pointCon_c*> getConnections(pointInfo_c* point);

    /// @brief find the points based on the num value of the point
    /// @param num_point : the num value for the points
    /// @return the target points address
    pointInfo_c* findPoint(uint32_t num_point);
    

    

    friend std::ostream& operator<< (std::ostream & out,const adjacencyMap &out_map);
    //TODO: add the code for cin


protected:


private:


    std::vector<pointInfo_c*> points;
    std::vector<pointCon_c*> connections;   

};


#endif