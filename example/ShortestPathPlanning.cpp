#include <iostream>
#include <string>
#include <libgen.h>
#include <unistd.h> // for getopt in decoding command line data

#include <gperftools/profiler.h> // for testing the code
#include <gperftools/heap-profiler.h>

#include "triangleCommand.h"
#include "adjacencyMap.h"
#include "Dijkstra.h"

static const std::string TrianglePath = "../ThirdParty/triangle/triangle";


static const std::string DefaultMapPath = "../tests/squareMapTest";
static const std::string DefaultMapName = "squareMapTest";
static const int START_POINT = 2;
static const int END_POINT = 4;

static const double DefaultAreaSet = 0.1;

class inputParameters
{
public:
    std::string MapPath;
    std::string MapName;
    unsigned int startPoint;
    unsigned int endPoint;

    /// Settings for the area mappings
    /// @brief the maximum area set for the triangle
    double areaSet;
    
    /// @brief decide if added the middle points to the map
    bool middlePoint;


    /// settings for the output data
    /// @brief decide if the software need to use python for display
    bool display;


public:
    /// @brief Enter the data from the command lines
    inputParameters(int argc = 0, char** argv = nullptr):
        MapPath(DefaultMapPath),MapName(DefaultMapName),
        startPoint(START_POINT),endPoint(END_POINT),
        areaSet(DefaultAreaSet),
        middlePoint(false),
        display(false)
    {
        int command;
        const char *optstring = "ha:df:"; 
        std::string parameter_string;
        while ((command = getopt(argc, argv, optstring)) != -1) {
            switch (command) {
                case 'h':
                    std::cout << "usage: ./ShortestPathPlanning [option] [arg] ... \n"
                              << "Options and arguments:\n"
                              << "-h          : help\n"
                              << "-a area     : maximum area for the triangulation \n"
                              << "-d          : enable display \n"
                              << "-f filepath : set the map path \n"
                              << "-m          : add the middle points to the map \n"
                              << std::endl;
                    exit(0);
                    break;
                case 'a':
                    parameter_string = std::string(optarg);    
                    areaSet = std::stod(parameter_string);
                    break;
                case 'd':
                    display = true;
                    break;
                case 'f':
                    MapPath = std::string(dirname(optarg));
                    MapName = std::string(basename(optarg));
                    break;
                case 'm':
                    middlePoint = true;
                    break;
                case '?':
                    printf("error optopt: %c\n", optopt);
                    printf("error opterr: %d\n", opterr);
                    break;
            }
        }
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


int main(int argc, char** argv)
{
    inputParameters commandInput(argc, argv);



    std::cout << commandInput << std::endl;

    // call the triangle
    triangleCommand triangle(TrianglePath);

    triangle.setParameter("a", commandInput.areaSet);

    triangle.call(commandInput.MapPath + "/" + commandInput.MapName +".poly");

#ifndef NDEBUG
    ProfilerStart("test.prof");
    HeapProfilerStart("heap.log");
#endif

    // analyse the data from triangle
    adjacencyMap map(commandInput.MapPath + "/" + commandInput.MapName + ".1.node", commandInput.MapPath + "/" + commandInput.MapName + ".1.ele");

    if(commandInput.middlePoint == true)
    {
        std::cout << "add middle points" << std::endl;
        map.addMiddlePoints();
    }


    // Dijkstra algorithm start

    Dijkstra path_planing;
    path_planing.setAdjacencyMap(&map);



    path_planing.calculateShortestPath(START_POINT, END_POINT);

#ifndef NDEBUG
    HeapProfilerDump("");
    HeapProfilerStop();
    ProfilerFlush();
    ProfilerStop();
#endif

    // result output
    std::string resultFile = commandInput.MapPath + "/" + "Result.poly";

    path_planing.exportToPolyFile(resultFile);

    std::cout << "Shortest Distance: " << float(path_planing.getPathCost()) << std::endl;
    
    if(commandInput.display)
    {
        std::cout << path_planing;
        system((std::string("/bin/python3 ../tools/displayPath.py -f ") + resultFile).c_str());
    }

    return 0;
}
