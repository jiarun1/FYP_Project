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
    fermatPoint(nullptr), middlePoint(nullptr), circumCenter(nullptr), orthoCenter(nullptr),inCenter(nullptr)
{
    nodes[0] = node_1;
    nodes[1] = node_2;
    nodes[2] = node_3;
}


triangle_c::~triangle_c()
{
    // if(fermatPoint!= nullptr)
    // {delete fermatPoint;}

    // if(middlePoint!= nullptr)
    // {delete middlePoint;}

    // if(circumCenter!= nullptr)
    // {delete circumCenter;}

    // if(orthoCenter!= nullptr)
    // {delete orthoCenter;}

    // if(inCenter!= nullptr)
    // {delete inCenter;}
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

segment_c* triangle_c::getAnotherSegments(point_c* node)
{
    for(int i = 0; i < 3; i++)
    {
        if(node == nodes[i])
        {
            for(int j = 0; j < 3; j++)
            {
                if((connections[j]->node1 != node) && (connections[j]->node2 != node))
                {
                    return connections[j];
                }
            }
        }
    }

    std::cerr << "triangle_c::getAnotherNode() : the input node is not part of triangle" << std::endl;
    return NULL;
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


/// This algorithm is used to calculate the tc used to calculate the Fermat point
ACCURACY triangle_c::tc_Fermat(ACCURACY area, ACCURACY s, ACCURACY t, ACCURACY u)
{
    ACCURACY s_2 = s*s;
    ACCURACY t_2 = t*t;
    ACCURACY u_2 = u*u;


    /// sqrt(3) use 1.73205
    ACCURACY m = 4*area + SQRT_3*(s_2 + t_2 - u_2);
    ACCURACY n = 4*area + SQRT_3*(s_2 - t_2 + u_2);
    ACCURACY v = 8*area*(12*area + SQRT_3*(s_2 + t_2 + u_2));
    return (m*n)/v;
}

ACCURACY triangle_c::maximumAngleCos()
{
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

    ACCURACY maximum_angle_cos = (edges[1]*edges[1] + edges[2]*edges[2] - edges[0]*edges[0])/(2*edges[1]*edges[2]);

    return maximum_angle_cos;
}

point_c* triangle_c::getFermatPoint()
{
    if(fermatPoint != nullptr){
        return fermatPoint;
    }

    // // find the maximum border
    // ACCURACY edges[3]; /// made edges[0] be the maxmimum edge in the triangle

    // edges[0] = connections[0]->distance; // initialize the maximum edge
    // for(int i = 1; i < 3; i++)
    // {
    //     if(connections[i]->distance > edges[0])
    //     {
    //         edges[i] = edges[0];
    //         edges[0] = connections[i]->distance;
    //     } else {
    //         edges[i] = connections[i]->distance;
    //     }
    // }

    // // decide if the fermat point is exist or not (has angle >= 120)
    // // use cos(C) = (a^2+b^2-c^2)/(2ab)
    // if( (- 0.5) > (edges[1]*edges[1] + edges[2]*edges[2] - edges[0]*edges[0])/(2*edges[1]*edges[2]))
    // {
    //     // std::cout << "Do not has fermat point" << std::endl;
    //     return nullptr;
    // }

    // decide if the fermat point is exist or not (has angle >= 120)
    // use cos(C) = (a^2+b^2-c^2)/(2ab)
    if( (- 0.5) > maximumAngleCos())
    {
        // std::cout << "Do not has fermat point" << std::endl;
        return nullptr;
    }

    
    // calculate the fermat point
    ACCURACY area = getArea();
    
    ACCURACY tc_Fermat_abc = tc_Fermat(area, connections[0]->distance, connections[1]->distance, connections[2]->distance);
    ACCURACY tc_Fermat_bca = tc_Fermat(area, connections[1]->distance, connections[2]->distance, connections[0]->distance);
    ACCURACY tc_Fermat_cab = tc_Fermat(area, connections[2]->distance, connections[0]->distance, connections[1]->distance);


    // calculate teh x and y
    ACCURACY x_fermat =  tc_Fermat_abc * getAnotherNode(connections[0])->x + 
                         tc_Fermat_bca * getAnotherNode(connections[1])->x +
                         tc_Fermat_cab * getAnotherNode(connections[2])->x;
    
    ACCURACY y_fermat =  tc_Fermat_abc * getAnotherNode(connections[0])->y + 
                         tc_Fermat_bca * getAnotherNode(connections[1])->y +
                         tc_Fermat_cab * getAnotherNode(connections[2])->y;


    // can use the matrix to improve the efficiency
    // ACCURACY x_fermat = tc_Fermat(area, connections[0]->distance, connections[1]->distance, connections[2]->distance) 
    //                         * getAnotherNode(connections[0])->x + 
    //                     tc_Fermat(area, connections[1]->distance, connections[2]->distance, connections[0]->distance) 
    //                         * getAnotherNode(connections[1])->x +
    //                     tc_Fermat(area, connections[2]->distance, connections[0]->distance, connections[1]->distance) 
    //                         * getAnotherNode(connections[2])->x;
    
    // ACCURACY y_fermat = tc_Fermat(area, connections[0]->distance, connections[1]->distance, connections[2]->distance) 
    //                         * getAnotherNode(connections[0])->y + 
    //                     tc_Fermat(area, connections[1]->distance, connections[2]->distance, connections[0]->distance) 
    //                         * getAnotherNode(connections[1])->y +
    //                     tc_Fermat(area, connections[2]->distance, connections[0]->distance, connections[1]->distance) 
    //                         * getAnotherNode(connections[2])->y;

    fermatPoint = new point_c(x_fermat, y_fermat, 0, point_c::fermat_point);

    return fermatPoint;
}


point_c* triangle_c::getMiddlePoint()
{
    if(middlePoint != nullptr)
    {
        return middlePoint;
    }

    ACCURACY x1 = nodes[0]->x;
    ACCURACY x2 = nodes[1]->x;
    ACCURACY x3 = nodes[2]->x;
    ACCURACY y1 = nodes[0]->y;
    ACCURACY y2 = nodes[1]->y;
    ACCURACY y3 = nodes[2]->y;


    ACCURACY x_middle = (x1 + x2 + x3)/3;
    ACCURACY y_middle = (y1 + y2 + y3)/3;

    // ACCURACY x_middle = (nodes[0]->x + nodes[1]->x + nodes[2]->x)/3;
    // ACCURACY y_middle = (nodes[0]->y + nodes[1]->y + nodes[2]->y)/3;

    middlePoint = new point_c(x_middle, y_middle, 0, point_c::middle_point);

    return middlePoint;
}

point_c* triangle_c::getCircumCenter()
{
    if(circumCenter != nullptr)
    {
        return circumCenter;
    }
    // decide if the decide if the circumcenter is inside the triangle or not
    // use cos(90) = 0 = (a^2+b^2-c^2)/(2ab)
    if( (0) > maximumAngleCos())
    {
        // std::cout << "circumcenter not inside triangle" << std::endl;
        return nullptr;
    }


    ACCURACY x1 = nodes[0]->x;
    ACCURACY x2 = nodes[1]->x;
    ACCURACY x3 = nodes[2]->x;
    ACCURACY y1 = nodes[0]->y;
    ACCURACY y2 = nodes[1]->y;
    ACCURACY y3 = nodes[2]->y;

    ACCURACY x_circ = (  x1*x1*y2 - x1*x1*y3 - x2*x2*y1 + x2*x2*y3 + x3*x3*y1 - x3*x3*y2 + y1*y1*y2 - y1*y1*y3 - y1*y2*y2 + y1*y3*y3 + y2*y2*y3 - y2*y3*y3)
                            /(2*(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2));
    ACCURACY y_circ = (- x1*x1*x2 + x1*x1*x3 + x1*x2*x2 - x1*x3*x3 + x1*y2*y2 - x1*y3*y3 - x2*x2*x3 + x2*x3*x3 - x2*y1*y1 + x2*y3*y3 + x3*y1*y1 - x3*y2*y2)
                            /(2*(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2));


    circumCenter = new point_c(x_circ, y_circ, 0, point_c::circumcenter);

    return circumCenter;

}

point_c* triangle_c::getOrthoCenter()
{
    if(orthoCenter != nullptr)
    {
        return orthoCenter;
    }
    // decide if the decide if the circumcenter is inside the triangle or not
    // use cos(90) = 0 = (a^2+b^2-c^2)/(2ab)
    if( (0) > maximumAngleCos())
    {
        // std::cout << "Orthocenter not inside triangle" << std::endl;
        return nullptr;
    }


    ACCURACY x1 = nodes[0]->x;
    ACCURACY x2 = nodes[1]->x;
    ACCURACY x3 = nodes[2]->x;
    ACCURACY y1 = nodes[0]->y;
    ACCURACY y2 = nodes[1]->y;
    ACCURACY y3 = nodes[2]->y;

    ACCURACY x_ortho = (y1*y2*y2 - y1*y1*y2 - y1*y3*y3 + y1*y1*y3 + y2*y3*y3 - y2*y2*y3 - x1*x2*y1 + x1*x2*y2 + x1*x3*y1 - x1*x3*y3 - x2*x3*y2 + x2*x3*y3)
                            /(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
    ACCURACY y_ortho = -(x1*x2*x2 - x1*x1*x2 - x1*x3*x3 + x1*x1*x3 + x2*x3*x3 - x2*x2*x3 - x1*y1*y2 + x1*y1*y3 + x2*y1*y2 - x2*y2*y3 - x3*y1*y3 + x3*y2*y3)
                            /(x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);


    orthoCenter = new point_c(x_ortho, y_ortho, 0, point_c::orthocenter);

    return orthoCenter;

}


point_c* triangle_c::getInCenter()
{
    if(inCenter != nullptr)
    {
        return inCenter;
    }

    // the length of each segments for the triangle
    ACCURACY a = connections[0]->distance, b = connections[1]->distance, c = connections[2]->distance;

    // the cooresponding nodes for the segments
    point_c* A = getAnotherNode(connections[0]),* B = getAnotherNode(connections[1]),* C = getAnotherNode(connections[2]);


    ACCURACY x = (a*A->x + b*B->x + c*C->x) / (a + b + c);
    ACCURACY y = (a*A->y + b*B->y + c*C->y) / (a + b + c);

    inCenter = new point_c(x, y, 0, point_c::incenter);

    return inCenter;

}