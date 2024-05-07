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

static const double DefaultAreaSet = 10;
static const double DefaultAngleSet = 0;

class inputParameters
{
public:
    std::string MapPath;
    std::string MapName;
    unsigned int startPoint;
    unsigned int endPoint;

    unsigned int parallel;

    /// Settings for the area mappings
    /// @brief the maximum area set for the triangle
    double areaSet;

    /// Settings for the minimum angle for triangle
    /// @brief the minimum angle set for the triangle
    double angleSet;

    /// @brief enable the CDT (only the constrain the triangle included)
    bool enableCDT;
    
    /// @brief decide if added the middle points to the map
    bool middlePoint;

    /// @brief decide if added the fermat points to the map
    bool fermatPoint;

    /// @brief add the circumcenter to the map
    bool circumcenter; 

    /// @brief add the orthocenter to the map
    bool orthocenter; 

    /// @brief add the incenter to the map
    bool incenter; 

    /// settings for the output data
    /// @brief decide if the software need to use python for display
    bool display;


public:
    /// @brief Enter the data from the command lines
    inputParameters(int argc = 0, char** argv = nullptr):
        parallel(1),
        MapPath(DefaultMapPath),MapName(DefaultMapName),
        startPoint(START_POINT),endPoint(END_POINT),
        areaSet(DefaultAreaSet),enableCDT(false),angleSet(DefaultAngleSet),
        middlePoint(false),fermatPoint(false),circumcenter(false),orthocenter(false),incenter(false),
        display(false)
    {
        int command;
        const char *optstring = "hp:a:q:Ddf:s:e:MFCOI"; 
        std::string parameter_string;
        while ((command = getopt(argc, argv, optstring)) != -1) {
            switch (command) {
                case 'h':
                    std::cout << "usage: ./ShortestPathPlanning [option] [arg] ... \n"
                              << "Options and arguments:\n"
                              << "-h          : help\n"
                              << "-p          : start parallel\n"
                              << "-a area     : maximum area for the triangulation \n"
                              << "-q          : minimum angle for the triangulation \n"
                              << "-d          : enable display \n"
                              << "-D          : set conforming delaunary triangulation \n"
                              << "-f filepath : set the map path \n"
                              << "-M          : (upper letter) add the middle points to the map \n"
                              << "-F          : (upper letter) add the fermat points to the map \n"
                              << "-C          : (upper letter) add the circumcenter \n"
                              << "-O          : (upper letter) add the orthocenter \n"
                              << "-I          : (upper letter) add the incenter \n"
                              << "-s          : set the start point \n"
                              << "-e          : set the end point \n"
                              << "\n"
                              << "********************************************************************"
                              << "Example: \n"
                              << "1.  ./ShortestPathPlanning -f ../tests/squareMapTest/squareMapTest.poly -a 3 -d -O"
                              << std::endl;
                    exit(0);
                    break;
                case 'a':
                    parameter_string = std::string(optarg);    
                    areaSet = std::stod(parameter_string);
                    break;
                case 'p':
                    parameter_string = std::string(optarg);    
                    parallel = std::stoi(parameter_string);
                    break;
                case 'q':
                    parameter_string = std::string(optarg);    
                    angleSet = std::stod(parameter_string);
                    break;
                case 'd':
                    display = true;
                    break;
                case 'D':
                    enableCDT = true;
                    break;
                case 'f':
                    MapPath = std::string(dirname(optarg));
                    MapName = std::string(basename(optarg));
                    break;
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
                case 's':
                    parameter_string = std::string(optarg);
                    startPoint = std::stoi(parameter_string);
                    break;
                case 'e':
                    parameter_string = std::string(optarg);
                    endPoint = std::stoi(parameter_string);
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
            << "  - maximum area : "  << out_paraneter.areaSet << std::endl
            << "  - minimum angle: "  << out_paraneter.angleSet << std::endl
            << "  - display      : "  << ((out_paraneter.display)? std::string("true"):std::string("false")) << std::endl
            << "  - map file     : "  << out_paraneter.MapPath + "/" + out_paraneter.MapName << std::endl
            << std::endl;
        return out;
    }

    ~inputParameters(){};
};


int main(int argc, char** argv)
{
    inputParameters commandInput(argc, argv);

    // //TODO: bug test
    // commandInput.parallel = 2;
    // commandInput.areaSet = 10;
    // commandInput.angleSet = 9;


    std::cout << commandInput << std::endl;

    // call the triangle
    triangleCommand triangle(TrianglePath);

    // triangle.setParameter("a", commandInput.areaSet);
    triangle.setMaxTriangleArea(commandInput.areaSet);
    triangle.setMinTriangleAngle(commandInput.angleSet);
    triangle.setConformingDelaunaryTriangle(commandInput.enableCDT);

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


    // Dijkstra algorithm start

    Dijkstra path_planing;
    path_planing.setAdjacencyMap(&map);


    if(commandInput.parallel == 1){
        path_planing.calculateShortestPath(commandInput.startPoint, commandInput.endPoint);
    } else if (commandInput.parallel == 2) {
        path_planing.calculateShortestPath_Bidirectional(commandInput.startPoint, commandInput.endPoint);
        // path_planing.calculateShortestPathInParallel(commandInput.startPoint, commandInput.endPoint);
    }


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

    // triangle.cleanTmpFiles(commandInput.MapPath + "/" + commandInput.MapName +".poly");

    return 0;
}
