/***********************************************************
 * @file      : costMap.cpp
 * @brief     : This file is used to represent a unidirecti-
 *              onal map that is used to store the data for 
 *              the shortest path algorithm, it contains a 
 *              matrix for shortest path algorithm
 *              
 * @copyright : Copyright Jiarun LIU
 * @author    : Jiarun LIU
 * @version   : V3.1
***********************************************************/
#include "adjacencyMap.h"

#include <iomanip>  
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief: constructure the map
*/
adjacencyMap::adjacencyMap()
{

}

adjacencyMap::adjacencyMap(std::string nodefile, std::string elefile)
{
    readNodes(nodefile);
    readEles(elefile);
}

adjacencyMap::adjacencyMap(adjacencyMap& copy)
{

    //TODO: add the copying code
    clear();
    for(int i = 0; i < copy.points.size(); i++)
    {
        points.push_back(copy.points.at(i));
    }
}

adjacencyMap::~adjacencyMap()
{
    clear();
}

pointInfo_c* adjacencyMap::findPoint(POINT_NUM_MAX num_point)
{
    // if the point is at n, the system would be faster
    if(num_point <= points.size())
    {
        if(points.at(num_point-1)->num == num_point)
        {
            return points.at(num_point-1);
        }
    }


    // search all the points for the data
    std::vector<pointInfo_c*>::iterator it = points.begin();

    for(; it != points.end(); it++)
    {
        if((*it)->num == num_point)
        {
            return *it;
        }
    }
    return nullptr;
}

void adjacencyMap::readNodes(std::string nodefile)
{
    using namespace std;
    ifstream fin;
	fin.open(nodefile, ios::in);
	if (!fin.is_open())
	{
		cerr << "can't find the node file" << endl;
		return;
	}

    // read the data line by line
    POINT_NUM_MAX count_lines = 0;
    string line_tmp;
    POINT_NUM_MAX pointNum = 0;
    while(getline(fin, line_tmp))
    {
        // remove the comments from the input file
        int index = line_tmp.find('#');
        if(index != string::npos)
        {
            line_tmp = line_tmp.substr(0,index);
        }

        istringstream str(line_tmp);

        if(count_lines == 0)
        {
            // the first line of node file, indicate the number of nodes and information
            points.clear();
            str >> pointNum;
        } else if (count_lines > pointNum){
            break;
        } else {
            auto pointNew = new pointInfo_c;
            str >> pointNew->num;
            str >> pointNew->x;
            str >> pointNew->y;
            str >> pointNew->is_boundary;
            points.push_back(pointNew);
        }
        count_lines ++;
    }    
	fin.close();
}


void adjacencyMap::readEles(std::string elefile)
{
    using namespace std;
    
    if(points.size() == 0)
    {
        cerr << "Please import the node file first" << endl;
        return;
    }

    ifstream fin;
	fin.open(elefile, ios::in);
	if (!fin.is_open())
	{
		cerr << "can't find the ele file" << endl;
		return;
	}

    // read the data line by line
    POINT_NUM_MAX count_lines = 0;
    string line_tmp;
    POINT_NUM_MAX shapeNum = 0;
    POINT_NUM_MAX nodesPerShape = 0;

    while(getline(fin, line_tmp))
    {
        // remove the comments from the input file
        int index = line_tmp.find('#');
        if(index != string::npos)
        {
            line_tmp = line_tmp.substr(0,index);
        }

        istringstream str(line_tmp);

        if(count_lines == 0)
        {
            // decode the first line
            str >> shapeNum;
            str >> nodesPerShape;
            

            if(nodesPerShape != 3)
            {
                cerr << "The input ele file is not for triangle, the result might not work" << endl;
            }


        } else if (count_lines > shapeNum){
            break;
        } else {
            POINT_NUM_MAX shape_num = 0;
            str >> shape_num;
            POINT_NUM_MAX* node_list = new POINT_NUM_MAX[3];

            for(int i = 0; i < 3; i++)
            {
                str >> node_list[i];
            }

            triangle_c* triangle_new = new triangle_c(findPoint(node_list[0]),
                                                      findPoint(node_list[1]),
                                                      findPoint(node_list[2]));

            for(int i = 0; i < 3; i++)
            {
                // use % operator to makesure the 3 point are reached
                auto connection_new = triangle_new->nodes[i % 3]->addConnection(triangle_new->nodes[(i+1)%3]);
                if(connection_new != nullptr)
                {
                    connections.push_back(connection_new);
                }
            }

            triangle_new->initConnections();

            triangles.push_back(triangle_new);

            delete[] node_list;
        }
        count_lines ++;
    }    
	fin.close();
}



void adjacencyMap::addMiddlePoints()
{   
    int current_connection_size = connections.size();
    
    /// get the middle point for all the connections
    for(int i = 0; i < current_connection_size; i++)
    {
        auto current_connection = connections.at(i);

        auto middle_point = current_connection->getMiddlePoint();
        auto point_1 = current_connection->node1;
        auto point_2 = current_connection->node2;
        
        // give the middle point a number
        middle_point->num = points.size() + 1;
        points.push_back(middle_point);

        // remove the connection from the point
        point_1->removeConnection(current_connection);

        // make the middle point connected to the nodes
        auto connection_1 = middle_point->addConnection(point_1);
        auto connection_2 = middle_point->addConnection(point_2);

        // change the old connection to the new one, so that other element in the original connection list would not change
        connections.at(i) = connection_1;
        connections.push_back(connection_2);

        // NOTE: it is impossible to delete the connection here since the triangle_c still need it
    }

    // connected all the middle point in a triangle
    // This part is the approach 1 (connected each middle point directly)
    // for(int i = 0; i < triangles.size(); i++)
    // {
    //     auto current_triangle = triangles.at(i);
    //     for(int i = 0; i < 3; i++)
    //     {
    //         auto connection_new = current_triangle->connections[i % 3]->middlePoint->addConnection(current_triangle->connections[(i+1) % 3]->middlePoint);
    //         connection_new->proporities = pointCon_c::middle_connection;
            
    //         connections.push_back(connection_new);
    //     }
    // }

    for(int i = 0; i < triangles.size(); i++)
    {
        auto current_triangle = triangles.at(i);
        for(int i = 0; i < 3; i++)
        {
            pointInfo_c* target_point = current_triangle->getAnotherNode(current_triangle->connections[i]);
            auto connection_new = current_triangle->connections[i % 3]->middlePoint->addConnection(target_point);
            connection_new->proporities = pointCon_c::middle_connection;
            
            connections.push_back(connection_new);
        }
    }
}



















void adjacencyMap::clear()
{
    // clear the points
    for(int i = 0 ; i < points.size(); i++)
    {
        delete points.at(i);
    }
    points.clear();

    // clear the connections
    for(int i = 0 ; i < connections.size(); i++)
    {
        delete connections.at(i);
    }
    connections.clear();

    // clear the triangles
    for(int i = 0 ; i < triangles.size(); i++)
    {
        delete triangles.at(i);
    }
    triangles.clear();
        
}


ACCURACY adjacencyMap::getCost(pointCon_c* connection)
{
    return connection->distance;
}

POINT_NUM_MAX adjacencyMap::getPointNum()
{
    return points.size();
}

POINT_NUM_MAX adjacencyMap::getPathNum()
{
    return connections.size();
}

std::vector<pointCon_c*> adjacencyMap::getConnections(pointInfo_c* point)
{
    return point->connection;
}

std::vector<pointCon_c*> adjacencyMap::getAllConnections()
{
    return connections;
}

std::vector<pointInfo_c*> adjacencyMap::getAllPoints()
{
    return points;
}






/**
 * @brief: Display the unidirectional map through the cout
*/
std::ostream& operator<< (std::ostream & out,const adjacencyMap &out_map)
{
    out << "";
    for(int i = 0; i < out_map.points.size(); i++)
    {
        for(int j = 0; j < out_map.points.at(i)->connection.size();j++)
        {
            out << out_map.points.at(i)->connection.at(j)->node1 << " - ";
            out << out_map.points.at(i)->connection.at(j)->node2 << " - \n";
        }
    }

    return out;
}
