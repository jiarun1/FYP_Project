/***********************************************************
 * \file      : shortestPathAlgorithm.cpp
 * \brief     : This file contained a parent class for all 
 *              the shortest path algorithms
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/


#include "shortestPathAlgorithm.h"

shortestPathAlgorithm::shortestPathAlgorithm()
{

}


shortestPathAlgorithm::~shortestPathAlgorithm()
{

}


void shortestPathAlgorithm::calculateShortestPath()
{
    
}

void shortestPathAlgorithm::calculateShortestPath(uint16_t startPoint, uint16_t endPoint)
{
    start_point = startPoint;
    end_point = endPoint;
}

void shortestPathAlgorithm::setAdjacencyMap(adjacencyMap* map)
{
    cost_map = map;
}


adjacencyMap* shortestPathAlgorithm::getAdjacencyMap()
{
    return cost_map;
}

uint16_t* shortestPathAlgorithm::getPath()
{
    return path;
}

uint16_t shortestPathAlgorithm::getPathPointNum()
{
    return path_point_num;
}

adjacencyMap::DISTANCE_ACCURACY shortestPathAlgorithm::getPathCost()
{
    return path_cost;
}

void shortestPathAlgorithm::exportToPolyFile(std::string file_path)
{
    using namespace std;
    fstream file;
	file.open(file_path, std::ios::ate | std::ios::out);
	if (!file.is_open())
	{
		cerr << "can't write the result file" << endl;
		return;
	}

    uint32_t point_num = cost_map->getPointNum();

    /******************************************************/
    // Point Section
    file << point_num << " "  // point number
         << 2 << " "    // dimension, should be 2
         << 1 << " "    // attribute number
         << 0 << endl;  // boundary number

    // vertex information
    for(int i = 0; i < point_num; i++)
    {
        file << i + 1 << " " // vertex number
             << cost_map->getPointPosition(i)->x << " "
             << cost_map->getPointPosition(i)->y << " ";

        int j = 0;
        for(j = 0; j < path_point_num; j++)
        {
            if(path[j] == i)
            {
                file << 1 << endl;
                break;
            }
        }
        if(j >= path_point_num)
        {
            file << 0 << endl;
        }
    }

    /******************************************************/
    // Line Section
    uint32_t path_num = cost_map->getPathNum()/2; // the path is unidirectional

    file << path_num << " "  // point number
         << 0 << endl;  // boundary number

    int path_count = 1;
    for(int i = point_num - 1; i >= 0; i--)
    {
        for(int j = 0; j < i; j++)
        {
            if(cost_map->getCost(i,j) == ADJACENT_NO_PATH){
                continue;
            } else {
                file << path_count << " " << i << " " << j << endl;
                path_count++;
            }
        }
    }
    
    /******************************************************/
    // Hole Section
    file << 0 << endl;
    
    file.close();
}


/**
 * @brief: Display the unidirectional map through the cout
*/
std::ostream& operator<< (std::ostream & out,const shortestPathAlgorithm &outPath)
{
    out << "path:";
    for(int i = 0; i < outPath.path_point_num; i++)
    {
        out << outPath.path[i] << "->";
    }
    out << "\n";
    return out;
}