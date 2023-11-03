cmake_minimum_required(VERSION 2.8)

# Define project name
project(ShortestPathPlanning)

# set execution file path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# set the include directionary
include_directories(${PROJECT_SOURCE_DIR}/include)


link_libraries(pthread)

# Include OpenCV
#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})

# Generate the execution files
add_executable(ShortestPathPlanning ./ShortestPathPlanning.cpp)


# Link librarys
#target_link_libraries(cameraserver ${OpenCV_LIBS})