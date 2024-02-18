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
// CLASS: point_c
//----------------------------------------------------------
point_c::point_c(proporities_e porp):
    proporities(porp)
{

}


point_c::point_c(ACCURACY x, ACCURACY y, ACCURACY z, proporities_e prop):
    x(x), y(y), z(z), proporities(prop)
{
    
}

point_c::~point_c()
{

}


segment_c* point_c::addConnection(point_c* adjacentPoint)
{
    segment_c* connection_new = this->findConnected(adjacentPoint);

    // if the connection is already exist, return a null
    if(connection_new != nullptr)
    {
        return nullptr;
    }
    
    connection_new = new segment_c(this, adjacentPoint);
    this->connection.push_back(connection_new);
    adjacentPoint->connection.push_back(connection_new);
    return connection_new;
}

bool point_c::removeConnection(segment_c* connection)
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


bool point_c::isConnected(point_c* adjacentPoint)
{
    if(findConnected(adjacentPoint) == nullptr)
    {
        return false;
    } else {
        return true;
    }
}

segment_c* point_c::findConnected(point_c* adjacentPoint)
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
// CLASS: segment_c
//--------------------------------------------------------------

segment_c::segment_c(point_c* node_1, point_c* node_2):
    node1(node_1), node2(node_2), middlePoint(NULL), proporities(normal_connection)
{
    // update and set the distance data
    distance = getDistance(node1,node2);
}



segment_c::~segment_c()
{

}

double segment_c::getDistance()
{
    return getDistance(node1, node2);
}

double segment_c::getDistance(point_c* node_1, point_c* node_2)
{
    float x_dif = node_1->x - node_2->x;
    float y_dif = node_1->y - node_2->y;

    return (sqrt(x_dif*x_dif+y_dif*y_dif));
}

point_c* segment_c::getMiddlePoint()
{
    if(middlePoint == NULL)
    {
        middlePoint = getMiddlePoint(node1, node2);
    }
    return middlePoint;
}

point_c* segment_c::getMiddlePoint(point_c* node_1, point_c* node_2)
{

    point_c* middle_point = new point_c(point_c::middle_point);

    middle_point->x = (node_1->x + node_2->x)/2;
    middle_point->y = (node_1->y + node_2->y)/2;
    middle_point->z = (node_1->z + node_2->z)/2;

    return middle_point;
}


point_c* segment_c::getAnotherNode(point_c* current_node)
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
// CLASS: triangle c
//--------------------------------------------------------------
triangle_c::triangle_c(point_c* node_1, point_c* node_2, point_c* node_3):
    fermatPoint(nullptr)
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
        
        if(connections[i] == nullptr)
        {
            connections[i] = nodes[i%3]->addConnection(nodes[(i+1)%3]);
        }

        nodes[i]->triangles.push_back(this);
        connections[i]->triangles.push_back(this);
    }
}


point_c* triangle_c::getAnotherNode(point_c* node_1, point_c* node_2)
{
    point_c* another_node = NULL;
    bool if_node1 = false, if_node2 = false;

    for(int i = 0; i < 3; i++)
    {
        if((nodes[i] != node_1) && (nodes[i] != node_2)){
            another_node = nodes[i];
        } else if ((nodes[i] == node_1) && (nodes[i] != node_2)){
            if_node1 = true;
        } else if ((nodes[i] != node_1) && (nodes[i] == node_2)){
            if_node2 = true;
        } else if ((nodes[i] == node_1) && (nodes[i] == node_2)){
            std::cerr << "triangle_c::getAnotherNode() : both input is the same" << std::endl;
            return NULL; 
        }       
    }

    // both node is inside the triangle
    if((if_node1 == true) && (if_node2 == true))
    {
        return another_node;
    } else {
        std::cerr << "triangle_c::getAnotherNode() : one of the input node is not part of triangle" << std::endl;
        return NULL;
    }
}


point_c* triangle_c::getAnotherNode(segment_c* connection)
{
    return getAnotherNode(connection->node1, connection->node2);
}


triangle_c* triangle_c::getAdjacentTriangle(segment_c* connection)
{
    triangle_c* adjacent_triangle = NULL;
    bool is_in_triangle = false; ///< used to makesure the connection is in the triangle

    for(int i = 0; i < connection->triangles.size(); i++)
    {
        if(connection->triangles.at(i) != this)
        {
            adjacent_triangle = connection->triangles.at(i);
        } else {
            is_in_triangle = true;
        }
    }

    if(is_in_triangle == false)
    {
        std::cerr << "triangle_c::getAdjacentTriangle() : the connection is not an edge of the triangle" << std::endl;
        return NULL;
    } else {
        return adjacent_triangle;
    }
}


ACCURACY triangle_c::getArea()
{
    // abs((ya+yb)*(xb-xa) + (yb+yc)*(xc-xb) + (ya+yc)*(xa-xc))/2
    return abs((nodes[0]->y + nodes[1]->y)*(nodes[0]->x - nodes[1]->x) + 
               (nodes[1]->y + nodes[2]->y)*(nodes[1]->x - nodes[2]->x) + 
               (nodes[2]->y + nodes[0]->y)*(nodes[2]->x - nodes[0]->x))/2;
}


point_c* triangle_c::getFermatPoint()
{
    if(fermatPoint != nullptr){
        return fermatPoint;
    }

    // find the maximum border
    ACCURACY edges[3]; /// made edges[0] be the maxmimum edge in the triangle

    edges[0] = connections[0]->distance; // initialize the maximum edge
    for(int i = 1; i < 3; i++)
    {
        if(connections[i]->distance > edges[0])
        {
            edges[i] = edges[0];
            edges[0] = connections[i]->distance;
        } else {
            edges[i] = connections[i]->distance;
        }
    }

    // decide if the fermat point is exist or not (has angle >= 120)
    // use cos(C) = (a^2+b^2-c^2)/(2ab)
    if( (- 0.5) > (edges[1]*edges[1] + edges[2]*edges[2] - edges[0]*edges[0])/(2*edges[1]*edges[2]))
    {
        // std::cout << "Do not has fermat point" << std::endl;
        return nullptr;
    }
    
    // calculate the fermat point
    ACCURACY area = getArea();

    // can use the matrix to improve the efficiency
    ACCURACY x_fermat = tc_Fermat(area, connections[0]->distance, connections[1]->distance, connections[2]->distance) 
                            * getAnotherNode(connections[0])->x + 
                        tc_Fermat(area, connections[1]->distance, connections[2]->distance, connections[0]->distance) 
                            * getAnotherNode(connections[1])->x +
                        tc_Fermat(area, connections[2]->distance, connections[0]->distance, connections[1]->distance) 
                            * getAnotherNode(connections[2])->x;
    
    ACCURACY y_fermat = tc_Fermat(area, connections[0]->distance, connections[1]->distance, connections[2]->distance) 
                            * getAnotherNode(connections[0])->y + 
                        tc_Fermat(area, connections[1]->distance, connections[2]->distance, connections[0]->distance) 
                            * getAnotherNode(connections[1])->y +
                        tc_Fermat(area, connections[2]->distance, connections[0]->distance, connections[1]->distance) 
                            * getAnotherNode(connections[2])->y;

    fermatPoint = new point_c(x_fermat, y_fermat, 0, point_c::fermat_point);

    return fermatPoint;
}



ACCURACY triangle_c::tc_Fermat(ACCURACY area, ACCURACY s, ACCURACY t, ACCURACY u)
{
    /// sqrt(3) use 1.73205
    ACCURACY m = 4*area + SQRT_3*(s*s + t*t - u*u);
    ACCURACY n = 4*area + SQRT_3*(s*s - t*t + u*u);
    ACCURACY v = 8*area*(12*area + SQRT_3*(s*s + t*t + u*u));
    return (m*n)/v;
}