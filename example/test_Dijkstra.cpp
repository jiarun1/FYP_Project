#include "Dijkstra.h"
#include "costMap.h"
#include <iostream>

using namespace std;


int main(int argc, char** argv)
{
    costMap map(13);

////////////////////////////////////////////////////////////////
    /// initialize the map
    map.setCost(0,1,15);
    map.setCost(0,2,21);
    map.setCost(0,6,14);

    map.setCost(1,0,15);
    map.setCost(1,2,11);

    map.setCost(2,0,21);
    map.setCost(2,1,11);
    map.setCost(2,5,13);
    map.setCost(2,4,20);
    map.setCost(2,3,17);

    map.setCost(3,2,17);
    map.setCost(3,10,20);

    map.setCost(4,2,20);
    map.setCost(4,5,12);
    map.setCost(4,9,13);
    map.setCost(4,10,11);

    map.setCost(5,2,13);
    map.setCost(5,4,12);
    map.setCost(5,9,11);
    map.setCost(5,6,27);

    map.setCost(6,0,14);
    map.setCost(6,5,27);
    map.setCost(6,7,14);
    map.setCost(6,8,22);

    map.setCost(7,6,14);
    map.setCost(7,11,17);
    map.setCost(7,12,30);

    map.setCost(8,6,22);
    map.setCost(8,11,9);
    map.setCost(8,9,9);

    map.setCost(9,5,11);
    map.setCost(9,4,13);
    map.setCost(9,10,13);
    map.setCost(9,12,15);
    map.setCost(9,11,9);
    map.setCost(9,8,9);

    map.setCost(10,3,20);
    map.setCost(10,4,11);
    map.setCost(10,9,13);
    map.setCost(10,12,15);

    map.setCost(11,7,17);
    map.setCost(11,8,9);
    map.setCost(11,9,9);

    map.setCost(12,7,30);
    map.setCost(12,9,15);
    map.setCost(12,10,15);

    // cout << "Map" << endl;
    // cout << map << endl;

////////////////////////////////////////////////////////////////

    Dijkstra dijkstra;

    dijkstra.setCostMap(&map);

    dijkstra.calculateShortestPath(1,12);


    uint16_t path_len = dijkstra.getPathLength();

    auto path = dijkstra.getPath();

    cout << endl << endl << "Path:\t";
    for(int i = 0; i < path_len; i++)
    {
        cout << path[i] << '\t';
    }
    cout << endl;

    return 0;







}