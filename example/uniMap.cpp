#include <iostream>
#include "unidirectionalMap.h"

using namespace std;

int main(int argv, char** argc)
{
    unidirectionalMap test_map(10);

    cout << test_map;
    
    return 0;   
}