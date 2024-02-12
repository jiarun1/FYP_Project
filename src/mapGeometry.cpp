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
#include <iostream>

//--------------------------------------------------------------
// CLASS: pointCon_c
//--------------------------------------------------------------

pointCon_c::pointCon_c(pointInfo_c* node_1, pointInfo_c* node_2):
    node1(node_1), node2(node_2), middlePoint(NULL), proporities(normal_connection)
{
    // update and set the distance data
    distance = calDistance(node1,node2);
}



pointCon_c::~pointCon_c()
{

}

double pointCon_c::calDistance()
{
    return calDistance(node1, node2);
}

double pointCon_c::calDistance(pointInfo_c* node_1, pointInfo_c* node_2)
{
    float x_dif = node_1->x - node_2->x;
    float y_dif = node_1->y - node_2->y;

    return (sqrt(x_dif*x_dif+y_dif*y_dif));
}

pointInfo_c* pointCon_c::getMiddlePoint()
{
    if(middlePoint == NULL)
    {
        middlePoint = getMiddlePoint(node1, node2);
    }
    return middlePoint;
}

pointInfo_c* pointCon_c::getMiddlePoint(pointInfo_c* node_1, pointInfo_c* node_2)
{

    pointInfo_c* middle_point = new pointInfo_c(pointInfo_c::middle_point);

    middle_point->x = (node_1->x + node_2->x)/2;
    middle_point->y = (node_1->y + node_2->y)/2;
    middle_point->z = (node_1->z + node_2->z)/2;

    return middle_point;
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
pointInfo_c::pointInfo_c(proporities_e porp):
    proporities(porp)
{

}

pointInfo_c::~pointInfo_c()
{

}


pointCon_c* pointInfo_c::addConnection(pointInfo_c* adjacentPoint)
{
    pointCon_c* connection_new = this->findConnected(adjacentPoint);

    // if the connection is already exist, return a null
    if(connection_new != nullptr)
    {
        return nullptr;
    }
    
    connection_new = new pointCon_c(this, adjacentPoint);
    this->connection.push_back(connection_new);
    adjacentPoint->connection.push_back(connection_new);
    return connection_new;
}

bool pointInfo_c::removeConnection(pointCon_c* connection)
{
    for(int i = 0; i < this->connection.size(); i++)
    {
        if(this->connection.at(i) == connection)
        {
            this->connection.erase(std::next(this->connection.begin(), i));

            // if the connection is exist, then also remove it from the adjacent point
            connection->node1->removeConnection(connection);
            connection->node2->removeConnection(connection);
            return true;
        }
    }
    return false;
}


bool pointInfo_c::isConnected(pointInfo_c* adjacentPoint)
{
    if(findConnected(adjacentPoint) == nullptr)
    {
        return false;
    } else {
        return true;
    }
}

pointCon_c* pointInfo_c::findConnected(pointInfo_c* adjacentPoint)
{
    for(int i = 0; i < this->connection.size(); i++)
    {
        if(this->connection.at(i)->node1 == adjacentPoint)
        {
            return this->connection.at(i);
        }
        if(this->connection.at(i)->node2 == adjacentPoint)
        {
            return this->connection.at(i);
        }
    }
    return nullptr;
}


//--------------------------------------------------------------
// CLASS: triangle c
//--------------------------------------------------------------
triangle_c::triangle_c(pointInfo_c* node_1, pointInfo_c* node_2, pointInfo_c* node_3)
{
    nodes[0] = node_1;
    nodes[1] = node_2;
    nodes[2] = node_3;
}


triangle_c::~triangle_c()
{

}

void triangle_c::initConnections()
{
    for(int i = 0; i < 3; i++)
    {
        connections[i] = nodes[i%3]->findConnected(nodes[(i+1)%3]);

        nodes[i]->triangles.push_back(this);
        connections[i]->triangles.push_back(this);
    }
}


pointInfo_c* triangle_c::getAnotherNode(pointInfo_c* node_1, pointInfo_c* node_2)
{
    // TODO: add some code to verify both node is in this triangle
    for(int i = 0; i < 3; i++)
    {
        if((nodes[i] != node_1) && (nodes[i] != node_2))
        {
            return nodes[i];
        }
    }
}


pointInfo_c* triangle_c::getAnotherNode(pointCon_c* connection)
{
    return getAnotherNode(connection->node1, connection->node2);
}