/***********************************************************
 * \file      : triangleData.cpp
 * \brief     : This file contained a class that used to get
 *              the data from the software: Triangle
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/
#include "triangleData.h"

#include <fstream>
#include <iostream>
#include <sstream>


triangleData::triangleData(/* args */):
    nodeNum(0), shapeNum(0)
{
    
}

triangleData::triangleData(std::string node_file, std::string ele_file):
    nodeNum(0), shapeNum(0)
{
    import(node_file, ele_file);
}

triangleData::~triangleData()
{
}

void triangleData::import(std::string node_file, std::string ele_file)
{
    readNodes(node_file);
    readEles(ele_file);
}

void triangleData::readNodes(std::string node_file)
{
    using namespace std;
    ifstream fin;
	fin.open(node_file, ios::in);
	if (!fin.is_open())
	{
		cerr << "can't find the node file" << endl;
		return;
	}

    // read the data line by line
    uint16_t count_lines = 0;
    string line_tmp;
    while(getline(fin, line_tmp))
    {
        // remove the comments from the input file
        int index = line_tmp.find('#');
        if(index != string::npos)
        {
            line_tmp = line_tmp.substr(0,index);
        }

        istringstream str(line_tmp);

        if(count_lines == 0)
        {
            if(nodeNum != 0)
            {
                delete[] nodeList;
            }
            str >> nodeNum;
            nodeList = new node_t[nodeNum];
        } else if (count_lines > nodeNum){
            break;
        } else {
            str >> nodeList[count_lines-1].num;
            str >> nodeList[count_lines-1].x;
            str >> nodeList[count_lines-1].y;
            str >> nodeList[count_lines-1].is_boundary;
        }
        count_lines ++;
    }    
	fin.close();
}

void triangleData::readEles(std::string ele_file)
{
    using namespace std;
    
    if(nodeNum == 0)
    {
        cerr << "Please import the node file first" << endl;
        return;
    }


    ifstream fin;
	fin.open(ele_file, ios::in);
	if (!fin.is_open())
	{
		cerr << "can't find the ele file" << endl;
		return;
	}

    // read the data line by line
    uint16_t count_lines = 0;
    string line_tmp;
    while(getline(fin, line_tmp))
    {
        // remove the comments from the input file
        int index = line_tmp.find('#');
        if(index != string::npos)
        {
            line_tmp = line_tmp.substr(0,index);
        }

        istringstream str(line_tmp);
        uint16_t node_per_shape;

        if(count_lines == 0)
        {
            if(shapeNum != 0)
            {
                delete[] shapeList;
            }
            str >> shapeNum;
            str >> node_per_shape;
            shapeList = new shape_t[shapeNum];
        } else if (count_lines > shapeNum){
            break;
        } else {
            shapeList[count_lines-1].nodes_num = node_per_shape;
            shapeList[count_lines-1].node = new node_t*[node_per_shape];

            str >> shapeList[count_lines-1].num;
            for(int i  = 0; i < node_per_shape; i++)
            {
                uint16_t node_code;
                str >> node_code;
                node_t* node_addr = &(nodeList[node_code-1]);

                if(node_code == node_addr->num)
                {
                    shapeList[count_lines-1].node[i] = node_addr;
                } else {
                    // if the previous predict is wrong, manually search for the node
                    for(int i = 0; i < nodeNum; i++)
                    {
                        if(nodeList[i].num == node_code)
                        {
                            node_addr = &(nodeList[i]);
                            break;
                        }
                    }
                }
            }
        }
        count_lines ++;
    }    
	fin.close();
}

