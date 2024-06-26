#include <string>

#include <iostream>
#include <string>
#include <libgen.h>
#include <unistd.h> // for getopt in decoding command line data
#include <chrono>

#include <gperftools/profiler.h> // for testing the code
#include <gperftools/heap-profiler.h>

#include "triangleCommand.h"
#include "adjacencyMap.h"
#include "Dijkstra.h"

#define VERSION 4


static const std::string TrianglePath = "../ThirdParty/triangle/triangle";
static const std::string DefaultMapPath = "../tests/squareMapTest";
static const std::string DefaultMapName = "squareMapTest";
static const int START_POINT = 2;
static const int END_POINT = 4;
static const double DefaultAreaSet = 1;




class inputParameters
{
public:
    std::string MapPath;
    std::string MapName;
    std::string LogPathAndName;
    unsigned int startPoint;
    unsigned int endPoint;

    /// Settings for the area mappings
    /// @brief the maximum area set for the triangle
    double areaSet;
    
    /// @brief decide if added the middle points to the map
    bool middlePoint;

    /// @brief decide if added the fermat points to the map
    bool fermatPoint;

    /// @brief decide if added the fermat points to the map
    bool circumcenter;

    /// @brief add the orthocenter to the map
    bool orthocenter; 

    /// @brief add the incenter to the map
    bool incenter; 



    /// settings for the output data
    /// @brief decide if the software need to use python for display
    bool display;

    std::string Version;


public:
    /// @brief Enter the data from the command lines
    inputParameters(int argc = 0, char** argv = nullptr):
        MapPath(DefaultMapPath),MapName(DefaultMapName),
        startPoint(START_POINT),endPoint(END_POINT),
        areaSet(DefaultAreaSet),
        middlePoint(false),fermatPoint(false),circumcenter(false),orthocenter(false),incenter(false),
        display(false)
    {
        int command;
        const char *optstring = "hf:s:e:V:MFCOIm"; 
        std::string parameter_string;
        while ((command = getopt(argc, argv, optstring)) != -1) {
            switch (command) {
                case 'h':
                    std::cout << "usage: ./ShortestPathPlanning [option] [arg] ... \n"
                              << "Options and arguments:\n"
                              << "-h          : help\n"
                              << "-f filepath : set the map path \n"
                              << "-M          : (upper letter) add the middle points to the map \n"
                              << "-F          : (upper letter) add the fermat points to the map \n"
                              << "-C          : (upper letter) add the circumcenter \n"
                              << "-O          : (upper letter) add the orthocenter \n"
                              << "-I          : (upper letter) add the incenter \n"
                              << "-s          : set the start point \n"
                              << "-e          : set the end point \n"
                              << "-V          : version number"
                              << std::endl;
                    exit(0);
                    break;
                case 'f':
                    MapPath = std::string(dirname(optarg));
                    MapName = std::string(basename(optarg));
                    break;
                case 's':
                    parameter_string = std::string(optarg);
                    startPoint = std::stoi(parameter_string);
                    break;
                case 'e':
                    parameter_string = std::string(optarg);
                    endPoint = std::stoi(parameter_string);
                    break;
                case 'V':
                    Version = std::string(optarg);
                    break;
                case '?':
                    printf("error optopt: %c\n", optopt);
                    printf("error opterr: %d\n", opterr);
                    break;
                // extra points
                case 'M':
                    middlePoint = true;
                    break;
                case 'F':
                    fermatPoint = true;
                    break;
                case 'C':
                    circumcenter = true;
                    break;
                case 'O':
                    orthocenter = true;
                    break;
                case 'I':
                    incenter = true;
                    break;
            }
        }

        // The log file path and name
        LogPathAndName = MapPath + "/" + Version + "_Code_Test.csv";  
    };

    friend std::ostream& operator<< (std::ostream & out,const inputParameters &out_paraneter)
    {
        out << "Parameters Settings List:" << std::endl
            << "  - maximum area: " << out_paraneter.areaSet << std::endl
            << "  - display     : " << ((out_paraneter.display)? std::string("true"):std::string("false")) << std::endl
            << "  - map file    : " << out_paraneter.MapPath + "/" + out_paraneter.MapName << std::endl
            << std::endl;
        return out;
    }

    ~inputParameters(){};
};



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


int main(int argc, char** argv)
{
    inputParameters commandInput(argc, argv);

    // call the triangle with parameters
    triangleCommand triangle(TrianglePath);

    if(commandInput.Version.empty())
    {
        std::cerr << "need to input the version number" << std::endl;
        return -1;
    }

    if( access( commandInput.LogPathAndName.c_str(), F_OK ) != -1 )
    {
        std::cout << "The file is already exist. (press y to convert or n to stop)" << std::endl;
        
        char input;
        std::cin >> input;
        if(input != 'y')
        {
            std::cout << "Stop Program (change another log file name)" << std::endl;
            return 0;
        }
    }


    writeLog_c log(commandInput.LogPathAndName);


    float max_area = 20;
    float min_area = 1e-3;
    float area_divide = 1.0/1000.0+1;
    int count_loop = 0;

    auto start_time_1 = std::chrono::high_resolution_clock::now();
    auto triangle_time_1 = std::chrono::high_resolution_clock::now();
    auto dataImportConvertion_time_1 = std::chrono::high_resolution_clock::now();
    auto shortestPathAlgorithm_time_1 = std::chrono::high_resolution_clock::now();

    auto start_time_2 = std::chrono::high_resolution_clock::now();
    auto triangle_time_2 = std::chrono::high_resolution_clock::now();
    auto dataImportConvertion_time_2 = std::chrono::high_resolution_clock::now();
    auto shortestPathAlgorithm_time_2 = std::chrono::high_resolution_clock::now();

    for(float test_area = max_area; test_area > min_area; test_area = test_area / area_divide)
    {
        count_loop ++;
        if(count_loop >= 100000)
        {
            std::cout << count_loop << std::endl;
            break;
        }

        
        std::string stage1_file_path = commandInput.MapPath;
        std::string stage1_file_name = "Stage1";
        {
            triangle.setMaxTriangleArea(3);
            start_time_1 = std::chrono::high_resolution_clock::now();
            triangle.call(commandInput.MapPath + "/" + commandInput.MapName +".poly");
            
            triangle_time_1 = std::chrono::high_resolution_clock::now();

            // analyse the data from triangle
            adjacencyMap map(commandInput.MapPath + "/" + commandInput.MapName + ".1.node", commandInput.MapPath + "/" + commandInput.MapName + ".1.ele");

            // Dijkstra algorithm start
            dataImportConvertion_time_1 = std::chrono::high_resolution_clock::now();

            Dijkstra path_planing;
            path_planing.setAdjacencyMap(&map);



            path_planing.calculateShortestPath(commandInput.startPoint, commandInput.endPoint);

            shortestPathAlgorithm_time_1 = std::chrono::high_resolution_clock::now();

            path_planing.exportAdjacentToPolyFile(stage1_file_path + "/" + stage1_file_name + ".poly");
        }



                // triangle.setParameter("a", commandInput.areaSet);
        triangle.setMaxTriangleArea(test_area);

        start_time_2 = std::chrono::high_resolution_clock::now();
        triangle.call(stage1_file_path + "/" + stage1_file_name + ".poly");

        triangle_time_2 = std::chrono::high_resolution_clock::now();

        // analyse the data from triangle
        adjacencyMap map(stage1_file_path + "/" + stage1_file_name + ".1.node", stage1_file_path + "/" + stage1_file_name + ".1.ele");
        if(commandInput.middlePoint == true)
        {
            std::cout << "add middle points" << std::endl;
            map.addMiddlePoints_triangle();
        }

        if(commandInput.fermatPoint == true)
        {
            std::cout << "add fermat points" << std::endl;
            map.addFermatPoints();
        }

        if(commandInput.circumcenter == true)
        {
            std::cout << "add circumcenter" << std::endl;
            map.addCircumcenter();
        }

        if(commandInput.orthocenter == true)
        {
            std::cout << "add orthocenter" << std::endl;
            map.addOrthocenter();
        }

        if(commandInput.incenter == true)
        {
            std::cout << "add incenter" << std::endl;
            map.addIncenter();
        }

        dataImportConvertion_time_2 = std::chrono::high_resolution_clock::now();


        // Dijkstra algorithm start

        Dijkstra path_planing;
        path_planing.setAdjacencyMap(&map);
        path_planing.calculateShortestPath(1 , 2);

        shortestPathAlgorithm_time_2 = std::chrono::high_resolution_clock::now();



        // //-------------------------- Parameter Settings --------------------------------
        // triangle.setParameter("a", test_area);

        // //--------------------------- Start Count Time --------------------------------
        // start_time_1 = std::chrono::high_resolution_clock::now();

        // triangle.call(commandInput.MapPath + "/" + commandInput.MapName +".poly");

        // triangle_time_1 = std::chrono::high_resolution_clock::now();
        
        // // Data import and convertion start
        // adjacencyMap map(commandInput.MapPath + "/" + commandInput.MapName + ".1.node", commandInput.MapPath + "/" + commandInput.MapName + ".1.ele");

        // if(commandInput.middlePoint == true)
        // {
        //     map.addMiddlePoints_triangle();
        // }

        // if(commandInput.fermatPoint == true)
        // {
        //     map.addFermatPoints();
        // }


        // if(commandInput.circumcenter == true)
        // {
        //     map.addCircumcenter();
        // }

        // if(commandInput.orthocenter == true)
        // {
        //     map.addOrthocenter();
        // }

        // if(commandInput.incenter == true)
        // {
        //     map.addIncenter();
        // }


        // dataImportConvertion_time_1 = std::chrono::high_resolution_clock::now();
        // // Dijkstra algorithm start

        // Dijkstra path_planing;
        // path_planing.setAdjacencyMap(&map);
        // path_planing.calculateShortestPath(START_POINT, END_POINT);

        // shortestPathAlgorithm_time_1 = std::chrono::high_resolution_clock::now();

        //--------------------------- End Count Time --------------------------------
        auto mapping_duration = std::chrono::duration_cast<std::chrono::microseconds>(triangle_time_1 - start_time_1 + triangle_time_2 - start_time_2);
        auto convertion_duration = std::chrono::duration_cast<std::chrono::microseconds>(dataImportConvertion_time_1 - triangle_time_1 + start_time_2 - shortestPathAlgorithm_time_1 + dataImportConvertion_time_2 - triangle_time_2);
        auto shortestPathAlgorithm_duration = std::chrono::duration_cast<std::chrono::microseconds>(shortestPathAlgorithm_time_1 - dataImportConvertion_time_1 + shortestPathAlgorithm_time_2 - dataImportConvertion_time_2);

        log.write(test_area ,mapping_duration, convertion_duration ,shortestPathAlgorithm_duration, map.getPointNum(), map.getPathNum(), path_planing.getPathCost());
        
    }

    return 0;
}


