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

void shortestPathAlgorithm::calculateShortestPath(POINT_NUM_MAX startPoint, POINT_NUM_MAX endPoint)
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

PATH_POINT_NUM_MAX* shortestPathAlgorithm::getPath()
{
    return path;
}

PATH_POINT_NUM_MAX shortestPathAlgorithm::getPathPointNum()
{
    return path_point_num;
}

ACCURACY shortestPathAlgorithm::getPathCost()
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
         << 2 << " "          // dimension, should be 2
         << 2 << " "          // attribute number
         << 0 << endl;        // boundary number

    // vertex information
    for(int i = 0; i < point_num; i++)
    {
        file << i + 1 << " " // vertex number
             << cost_map->findPoint(i+1)->x << " "
             << cost_map->findPoint(i+1)->y << " ";

        int j = 0;
        for(j = 0; j < path_point_num; j++)
        {
            if(path[j] == i + 1)
            {
                file << 1 << " ";
                break;
            }
        }
        if(j >= path_point_num)
        {
            file << 0 << " ";
        }

        file << cost_map->findPoint(i+1)->proporities << " ";

        file << endl;
    }

    /******************************************************/
    // Line Section
    std::vector<pointCon_c*> availablePaths = cost_map->getAllConnections();
    
    file << availablePaths.size() << " "  // point number
         << 0 << endl;  // boundary number

    for(int i = 0; i < availablePaths.size(); i++)
    {
        file << i+1 << " " << availablePaths.at(i)->node1->num << " " << availablePaths.at(i)->node2->num << endl;
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