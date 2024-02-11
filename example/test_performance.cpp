#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include <gperftools/profiler.h>

#include "triangleCommand.h"
#include "adjacencyMap.h"
#include "Dijkstra.h"

#define VERSION 4

static const std::string GeneralPath = "../tests/squareMapTest/";
static const std::string MapName = "squareMapTest";
static const std::string TrianglePath = "../ThirdParty/triangle/triangle";
static const std::string MapPath = GeneralPath;
static const std::string LogFilePath = GeneralPath;
static const std::string LogFileName = "V4_middle_Code_Test.csv";

static const int START_POINT = 2;
static const int END_POINT = 4;


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
        file << "Area Set" << ','
             << "Test Count" << ',' 
             << "Mapping Time(us)" << ',' 
             << "Convertion Time(us)" << ',' 
             << "Shorest Path Time(us)" << ',' 
             << "Points Num" << "," 
             << "Path Num" << ","
             << "Result Distance" << std::endl;
    }
    ~writeLog_c()
    {
        file.close();
    }

    void write(float set_area, 
               std::chrono::microseconds mapping_time_us, 
               std::chrono::microseconds convertion_time_us , 
               std::chrono::microseconds shortestPath_time_us , 
               long long points_num, 
               long long path_num, 
               double min_dis)
    {
        file << set_area << "," 
             << line_count << ',' 
             << mapping_time_us.count() << ',' 
             << convertion_time_us.count() << ','
             << shortestPath_time_us.count() << ',' 
             << points_num << "," 
             << path_num << "," 
             << min_dis << std::endl;

        line_count++;
    }

};


int main(int argv, char** argc)
{
    // call the triangle with parameters
    triangleCommand triangle(TrianglePath);
    
    writeLog_c log(LogFilePath + LogFileName);


    float max_area = 20;
    float min_area = 1e-3;
    float area_divide = 1.0/1000.0+1;
    int count_loop = 0;

    auto start_time = std::chrono::high_resolution_clock::now();
    auto triangle_time = std::chrono::high_resolution_clock::now();
    auto dataImportConvertion_time = std::chrono::high_resolution_clock::now();
    auto shortestPathAlgorithm_time = std::chrono::high_resolution_clock::now();

    ProfilerStart("test.prof");

    for(float test_area = max_area; test_area > min_area; test_area = test_area / area_divide)
    {
        count_loop ++;
        if(count_loop >= 100000)
        {
            std::cout << count_loop << std::endl;
            break;
        }

        //-------------------------- Parameter Settings --------------------------------
        triangle.setParameter("a", test_area);

        //--------------------------- Start Count Time --------------------------------
        start_time = std::chrono::high_resolution_clock::now();

        triangle.call(MapPath + MapName +".poly");

        triangle_time = std::chrono::high_resolution_clock::now();
        
        // Data import and convertion start
        adjacencyMap map(MapPath + MapName + ".1.node", MapPath + MapName + ".1.ele");

        map.addMiddlePoints();

        dataImportConvertion_time = std::chrono::high_resolution_clock::now();
        // Dijkstra algorithm start

        Dijkstra path_planing;
        path_planing.setAdjacencyMap(&map);
        path_planing.calculateShortestPath(START_POINT, END_POINT);

        shortestPathAlgorithm_time = std::chrono::high_resolution_clock::now();

        //--------------------------- End Count Time --------------------------------
        auto mapping_duration = std::chrono::duration_cast<std::chrono::microseconds>(triangle_time - start_time);
        auto convertion_duration = std::chrono::duration_cast<std::chrono::microseconds>(dataImportConvertion_time - triangle_time);
        auto shortestPathAlgorithm_duration = std::chrono::duration_cast<std::chrono::microseconds>(shortestPathAlgorithm_time - dataImportConvertion_time);

        log.write(test_area ,mapping_duration, convertion_duration ,shortestPathAlgorithm_duration, map.getPointNum(), map.getPathNum(), path_planing.getPathCost());
        
    }
    ProfilerStop();

    return 0;
}

