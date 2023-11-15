#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include <google/profiler.h>

#include "triangleCommand.h"
#include "triangleData.h"
#include "adjacencyMap.h"
#include "Dijkstra.h"

class writeLog_c
{
private:
    std::string log_file_path;
    std::fstream file;
    long long line_count;
    

public:
    writeLog_c(std::string file_path):
        line_count(1)
    {
        log_file_path = file_path;
        file.close();
        file.clear();
        file.open(file_path, std::ios::ate | std::ios::out);
        if (!file.is_open())
        {
            std::cerr << "can't write to this file" << std::endl;
            return;
        }
        file << "Line Num" << ' ' << "Time(us)" << ' ' << "Points Num" << " " << "Path Num" << " " << "Result Distance" << std::endl;
    }
    ~writeLog_c()
    {

    }

    void write(std::chrono::microseconds time_us, long long points_num, long long path_num, double min_dis)
    {
        line_count++;
        file << line_count << ' ' << time_us.count() << ' ' << points_num << " " << path_num << " " << min_dis << std::endl;
    }

};


int main(int argv, char** argc)
{
    // call the triangle with parameters
    triangleCommand triangle("../ThirdParty/triangle/triangle");
    
    writeLog_c log("../tests/performance_test_1/log-1.txt");


    float max_area = 400;
    float area_dif = 1;

    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);


    ProfilerStart("test.prof");

    for(float test_area = max_area; test_area > 20; test_area -= area_dif)
    {
        //-------------------------- Parameter Settings --------------------------------
        triangle.setParameter("a", test_area);

        //--------------------------- Start Count Time --------------------------------
        start_time = std::chrono::high_resolution_clock::now();



        triangle.call("../tests/performance_test_1/squareMapTest.poly");
        
        triangleData data;
        data.import("../tests/performance_test_1/squareMapTest.1.node", "../tests/performance_test_1/squareMapTest.1.ele");

        adjacencyMap map;
        data.toAdjacencyMap(&map);

        Dijkstra path_planing;
        path_planing.setAdjacencyMap(&map);
        path_planing.calculateShortestPath(1,3);

        //--------------------------- End Count Time --------------------------------
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        log.write(duration, map.getPointNum(), map.getPathNum(), path_planing.getPathCost());
        
    }
    ProfilerStop();

    return 0;
}

