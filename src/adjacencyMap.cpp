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
    //TODO: add copying code
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
        } else if (count_lines > shapeNum){
            break;
        } else {
            POINT_NUM_MAX shape_num = 0;
            str >> shape_num;
            POINT_NUM_MAX* node_list = new POINT_NUM_MAX[nodesPerShape];
            for(int i = 0; i < nodesPerShape; i++)
            {
                str >> node_list[i];
            }

            pointInfo_c* currentPoints; 
            pointInfo_c* nextPoints;

            for(int i = 0; i < nodesPerShape - 1; i++)
            {
                currentPoints = findPoint(node_list[i]);
                nextPoints = findPoint(node_list[i+1]);
                auto connection_new = currentPoints->addConnection(nextPoints);
                if(connection_new != nullptr)
                {
                    connections.push_back(connection_new);
                }
            }
            currentPoints = findPoint(node_list[nodesPerShape-1]);
            nextPoints = findPoint(node_list[0]);
            auto connection_new = currentPoints->addConnection(nextPoints);
            if(connection_new != nullptr)
            {
                connections.push_back(connection_new);
            }
            delete[] node_list;
        }
        count_lines ++;
    }    
	fin.close();
}


void adjacencyMap::addMiddlePoint()
{   
    
    for(int i = 0; i < connections.size(); i++)
    {
        pointInfo_c middle_point = connections.at(i)->getMiddlePoint();

    }
}



















void adjacencyMap::clear()
{
    //TODO: add clear code
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
