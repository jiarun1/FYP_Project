/***********************************************************
 * @file      : mapGeometry.h
 * @brief     : 
 *              
 * @copyright : Copyright Jiarun LIU
 * @author    : Jiarun LIU
 * @version   : V3.1
***********************************************************/
#include "mapGeometry.h"
#include <math.h>

//--------------------------------------------------------------
// CLASS: pointCon_c
//--------------------------------------------------------------

pointCon_c::pointCon_c(pointInfo_c* node_1, pointInfo_c* node_2):
    node1(node_1), node2(node_2)
{
    // update and set the distance data
    distance = calDistance(node1,node2);
}



pointCon_c::~pointCon_c()
{

}



double pointCon_c::calDistance(pointInfo_c* node_1, pointInfo_c* node_2)
{
    float x_dif = node_1->x - node_2->x;
    float y_dif = node_1->y - node_2->y;

    return (sqrt(x_dif*x_dif+y_dif*y_dif));
}


pointInfo_c* pointCon_c::getAnotherNode(pointInfo_c* current_node)
{
    if(current_node == node1) {
        // the node 1 is the current node
        return node2;
    } else if (current_node == node2) {
        // the node 2 is the current node
        return node1;
    } else {
        // none of the node is the current node
        return nullptr;
    }
}




//--------------------------------------------------------------
// CLASS: pointInfo_c
//----------------------------------------------------------
pointInfo_c::pointInfo_c()
{

}

pointInfo_c::~pointInfo_c()
{

}


pointCon_c* pointInfo_c::addConnection(pointInfo_c* adjacentPoint)
{
    if(this->isConnected(adjacentPoint))
    {
        return nullptr;
    }
    
    auto connectionNew = new pointCon_c(this, adjacentPoint);
    this->connection.push_back(connectionNew);
    adjacentPoint->connection.push_back(connectionNew);
    return connectionNew;
}


bool pointInfo_c::isConnected(pointInfo_c* adjacentPoint)
{
    for(int i = 0; i < this->connection.size(); i++)
    {
        if(this->connection.at(i)->node1 == adjacentPoint)
        {
            return true;
        }
        if(this->connection.at(i)->node2 == adjacentPoint)
        {
            return true;
        }
    }
    return false;
}