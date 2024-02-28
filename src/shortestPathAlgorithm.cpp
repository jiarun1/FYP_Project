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
    std::vector<segment_c*> availablePaths = cost_map->getAllConnections();
    
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

void shortestPathAlgorithm::exportAdjacentToPolyFile(std::string file_path)
{
    using namespace std;
    fstream file;
	file.open(file_path, std::ios::ate | std::ios::out);
	if (!file.is_open())
	{
		cerr << "can't write the result file" << endl;
		return;
	}

    std::vector<triangle_c*> adjacentTriangles;
    std::vector<point_c*> adjacentPoints;
    std::vector<segment_c*> boundarys; //< every boundary can only be exist in one adjacent triangle

    // let the start and end point become the first and second point in the adjacentPoints list
    adjacentPoints.push_back(cost_map->findPoint(path[0]));
    adjacentPoints.push_back(cost_map->findPoint(path[path_point_num - 1]));

    // find all the adjacent triangle (not has repeat one)
    for(int i = 0; i < path_point_num; i++)
    {
        point_c* current_path_point = cost_map->findPoint(path[i]);
        std::vector<triangle_c* > current_triangle_list = current_path_point->triangles;

        for(int j = 0; j < current_triangle_list.size(); j++)
        {
            int k = 0;
            for(k = 0; k < adjacentTriangles.size(); k++)
            {
                if(adjacentTriangles[k] == current_triangle_list[j])
                {
                    break;
                }
            }

            if(k >= adjacentTriangles.size())
            {
                adjacentTriangles.push_back(current_triangle_list[j]);
            }
        }
    }

    // 
    for(int i = 0; i < adjacentTriangles.size(); i++)
    {
        triangle_c* current_triangle = adjacentTriangles.at(i);

        // add the points
        for(int point_index = 0; point_index < 3; point_index ++)
        {
            // code to add the point in the triangle to the final one
            point_c* current_adjacent_point = current_triangle->nodes[point_index];

            // find if the point is exist or not (if exist do nothing, if not add to the list)
            int k = 0;
            for(k = 0; k < adjacentPoints.size(); k++)
            {
                if(current_adjacent_point == adjacentPoints.at(k))
                {
                    break;
                }
            }
            if(k >= adjacentPoints.size())
            {
                adjacentPoints.push_back(current_adjacent_point);
            }
        }

        // add the segments
        for(int segment_index = 0; segment_index < 3; segment_index ++)
        {
            segment_c* current_segment = current_triangle->connections[segment_index];

            // only add the segment exist once in the adjacent connections
            int k = 0, boundary_size_tmp = boundarys.size();
            for( k = 0; k < boundary_size_tmp; k++)
            {
                // note: 1 segment can maximum exist in 2 triangle
                if(boundarys[k] == current_segment)
                {
                    boundarys.erase(boundarys.begin()+k);
                    k = 0;
                    break;
                }
            }
            if(k >= boundary_size_tmp)
            {
                boundarys.push_back(current_segment);
            }
        }
    }


    /******************************************************/
    // Point Section
    file << adjacentPoints.size() << " "  // point number
         << 2 << " "          // dimension, should be 2
         << 0 << " "          // attribute number
         << 0 << endl;        // boundary number

    // vertex information
    for(int i = 0; i < adjacentPoints.size(); i++)
    {
        file << i + 1 << " " // vertex number
             << adjacentPoints.at(i)->x << " "
             << adjacentPoints.at(i)->y << " ";

        file << endl;
    }

    /******************************************************/
    // Line Section

    file << boundarys.size() << " "  // point number
         << 1 << endl;  // boundary number

    for(int i = 0; i < boundarys.size(); i++)
    {
        file << i+1 << " ";
        
        for(int j = 0; j < adjacentPoints.size(); j++)
        {
            if(boundarys.at(i)->node1 == adjacentPoints.at(j))
            {
                file << j+1 << " ";
            }

            if(boundarys.at(i)->node2 == adjacentPoints.at(j))
            {
                file << j+1 << " ";
            }
        }
        
        file << 1 << endl;
    }

    /******************************************************/
    // Hole Section
    file << 0 << endl;
    
    file.close();
}


/**
 * @brief: Display the output path through the cout
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