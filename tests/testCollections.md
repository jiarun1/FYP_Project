# Test

## square map test
1. V1_Code_Test.csv: simple triditional methods test (calculate the adjacent distance multiple times)
2. V1_1_Code_Test.csv: calculate the adjacent distance only one times
3. V2_Code_Test.csv: use the fibonacci heap to improve accuracy
4. V3_1_Code_Test.csv: improve the storage structure of the adjacent map
5. V3_2_Code_Test.csv: add the test result by changing minimal angle setting
6. V3_3_Code_Test.csv: add the test result by changing the constrain Triangulation
7. V4_1_Code_Test.csv: use the middle point of the edges (connected adjacent middle point)
8. V4_2_Code_Test.csv: use the middle point of the edges (connected middle point with corresponding vertexs)
9. V4_3_Code_Test.csv: add the fermat point to the triangle
10. V4_3_1_Code_Test.csv: add the fermat point to the triangle (with the angle and constrain settings)
11. V4_4_Code_Test.csv: add the middle point(centriod point) to the triangle
   1. Command Code
     ```
     ./PerformanceAnalysis -f "../tests/squareMapTest/squareMapTest.poly" -M -V V4_4
     ```
12. V4_5_Code_Test.csv: add the circumcenter to the triangle
   1. Command Code
     ```
     ./PerformanceAnalysis -f ../tests/squareMapTest/squareMapTest.poly -V V4_5 -C
     ```
13. V4_6_Code_Test.csv: add the orthocenter to improve the efficiency
    1.  Command Code
     ```
     ./PerformanceAnalysis -f ../tests/squareMapTest/squareMapTest.poly -V V4_6 -O
     ```
14. V4_7_Code_Test.csv: add the incenter to improve the efficiency
    1.  Command Code
     ```
     ./PerformanceAnalysis -f ../tests/squareMapTest/squareMapTest.poly -V V4_7 -I
     ```
15. V5_1_Code_Test.csv: add the second stage to improve the time performance
    1.  Command Code
     ```
     ./multiStageperformanceAnalysis -f ../tests/squareMapTest/squareMapTest.poly -V V5_1
     ```