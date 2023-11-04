cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# Define project name
project(ShortestPathPlanning)

# set execution file path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# set the include directionary
include_directories(${PROJECT_SOURCE_DIR}/include)

# Get the source files and head files
FILE(GLOB_RECURSE CURRENT_INCLUDE include/*.hpp)
FILE(GLOB_RECURSE CURRENT_SOURCES src/*.cpp)

# Include OpenCV
#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})

# Generate the execution files
add_executable(ShortestPathPlanning ./example/ShortestPathPlanning.cpp ${CURRENT_INCLUDE} ${CURRENT_SOURCES})

# generate the example execution files
add_executable(Test_CostMap ./example/test_costMap.cpp ${CURRENT_INCLUDE} ${CURRENT_SOURCES})
add_executable(Test_Dijkstra ./example/test_Dijkstra.cpp ${CURRENT_INCLUDE} ${CURRENT_SOURCES})


# Link librarys
#target_link_libraries(cameraserver ${OpenCV_LIBS})