# FYP_Project
My undergraduate project

## File Structure
- bin: the execution files
- example: the example code for the system (contain the standard output program)
- src: the source code (for C++)
- include: the head file (for C++)
- tests: the example test map with the result for different version
  - [Detail information for different test result files](tests/testCollections.md)
- ThirdParty: the thirdparty software used in this project
- tools: the python script and matlab code for data processing
  - algorithm: the scipts for verify the result
  - resultAnalysis: the scripts for data analysis


## Note
1. This version of the code would not be able to input polygon that is not triangle anymore

## Version Selection
### V4-release
1. tried the incenter to improve the accuracy
2. analysis and compare the function for all extra points

### V4.3
1. use the middle point (centroid) to improve accuracy
2. tried the circumcenter to improve accuracy 
3. tried the orthocenter to improce the accuracy

### V4.2
1. use middle point to improve accuracy (connect middle point with correspond nodes)

### V4.1
1. use middle point to improve accuracy (connected all the middle point in a triangle) => not improve the accuracy


## Third Party Softwares for this project

### Triangle
1. used to get the delaunary triangulation


### veeMeshViewer
1. Used to view the poly format image
2. only for windows platform
