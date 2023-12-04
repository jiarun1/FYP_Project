#include <iostream>
#include <string>
#include "adjacencyMap.h"

#include "triangleCommand.h"
#include "triangleData.h"
#include "adjacencyMap.h"
#include "Dijkstra.h"

static const std::string TrianglePath = "../ThirdParty/triangle/triangle";


static const std::string DefaultMapPath = "../tests/squareMapTest/";
static const std::string DefaultMapName = "squareMapTest";
static const int START_POINT = 1;
static const int END_POINT = 3;

static const double DefaultAreaSet = 3;

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
    


    /// settings for the output data
    /// @brief decide if the software need to use python for display
    bool display;

public:
    /// @brief Enter the data from the command lines
    inputParameters(int argv = 0, char** argc = nullptr):
        MapPath(DefaultMapName),MapName(DefaultMapName),
        startPoint(START_POINT),endPoint(END_POINT),
        areaSet(DefaultAreaSet),
        display(false)
    {
        for(int i = 0; i < argv; i++)
        {
            std::string parameter_string(argc[i]);

            std::cout << parameter_string << std::endl;

            if(parameter_string == std::string("-a"))
            {
                i += 1;
                if(i >= argv){std::cerr << "Please enter area number after -a" << std::endl;abort;}
                parameter_string= std::string(argc[i]);    
                areaSet = std::stod(parameter_string);
                continue;
            } else if(parameter_string == std::string("-d")){
                display = true;
                continue;
            }

        }
    };

    friend std::ostream& operator<< (std::ostream & out,const inputParameters &out_paraneter)
    {
        out << "Parameters Settings List:" << std::endl
            << "  - maximum area: " << out_paraneter.areaSet << std::endl
            << "  - display     : " << ((out_paraneter.display)? std::string("true"):std::string("false")) << std::endl
            << std::endl;
        return out;
    }

    ~inputParameters(){};
};


int main(int argv, char** argc)
{
    inputParameters commandInput(argv, argc);

    std::cout << commandInput << std::endl;

    // call the triangle
    triangleCommand triangle(TrianglePath);

    triangle.setParameter("a", commandInput.areaSet);

    triangle.call(DefaultMapPath + DefaultMapName +".poly");
 
    // analyse the data from triangle
    triangleData data;
    data.import(DefaultMapPath + DefaultMapName + ".1.node", DefaultMapPath + DefaultMapName + ".1.ele");

    adjacencyMap map;
    data.toAdjacencyMap(&map);

    // Dijkstra algorithm start

    Dijkstra path_planing;
    path_planing.setAdjacencyMap(&map);
    path_planing.calculateShortestPath(START_POINT, END_POINT);

    path_planing.exportToPolyFile(DefaultMapPath + "Result.poly");

    std::cout << "Shortest Distance: " << float(path_planing.getPathCost()) << std::endl;
    
    if(commandInput.display)
    {
        std::cout << path_planing;
        system("/bin/python3 ../tools/displayPath.py");
    }

    return 0;
}
