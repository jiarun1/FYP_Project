#include <iostream>
#include "costMap.h"

using namespace std;

int main(int argv, char** argc)
{
    costMap test_map(10);

    test_map.setCost(4,6,20);

    cout << test_map << endl;
    
    return 0;   
}