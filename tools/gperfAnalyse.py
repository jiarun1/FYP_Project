# this file is used to analyse the gperftools and analyse the output
# all in ms unit

import os
import numpy as np

# set the gperftools samping frequency
gperfSampleFreq = 1000
# method to set the frequency export CPUPROFILE_FREQUENCY=<frequency>

areaSet = np.arange(0.1,0.01,-0.01)

class functionInfo:
    def __init__(self, name, samplesIn, samplesInandInChild):
        self.name = name
        self.sampleIn = samplesIn
        self.sampleInandInchild = samplesInandInChild


def replaceMultiSpace(string):
    while '  ' in string:
        string = string.replace('  ', ' ')
    return string



def readProf(filename):
    target_file = gperfToTxt(filename)
    Data = list()

    with open(target_file,"r") as f:
        line = f.readline()
        currentLine = 0
        while line:
            line = replaceMultiSpace(line)
            line = line.replace("\n","")
            # print(line)
            linesplit = line.split(" ")
            # print(linesplit)
            linePartsNum = len(linesplit)

            if(linePartsNum <= 0):
                line = f.readline()
                currentLine += 1   
                continue

            if(linesplit[0]=="Total:"):
                line = f.readline()
                currentLine += 1   
                samplesNum = int(linesplit[1])
                # print(samplesNum)
                continue
            
            Data.append(functionInfo(linesplit[6], linesplit[1], linesplit[4]))

            line = f.readline()
            currentLine += 1   
    
    f.close()
    return samplesNum, Data

def findSpecificFunctionData(datalist, functionName):
    for data in datalist:
        if(data.name == functionName):
            return data



def gperfToTxt(filename):
    os.system("pprof --text ../bin/ShortestPathPlanning ./test.prof > ./test.txt")
    return "./test.txt"


if __name__ == "__main__":
    # sampleNum, data = readProf("../bin/test.prof")
    # print(findSpecificFunctionData(data,"adjacencyMap::getCost").sampleIn)
    
    SpecificFunList = list()

    for area in areaSet:
        os.system("../bin/ShortestPathPlanning -a " + str(area))
        sampleNum, data = readProf("../test.prof")
        
        SpecificFun = findSpecificFunctionData(data,"adjacencyMap::getCost")
        SpecificFunList.append(SpecificFun)


    for fun in SpecificFunList:
        print(fun.sampleIn)

    

      
