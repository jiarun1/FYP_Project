/***********************************************************
 * \file      : triangleCommand.h
 * \brief     : This file contained a class that used call the 
 *              command line for the software: Triangle
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/
#ifndef __TRIANGLECOMMAND_H__
#define __TRIANGLECOMMAND_H__

#include <string>


class triangleCommand
{
public:
    triangleCommand(std::string software_path);
    ~triangleCommand();

    void call(std::string poly_path);

    void clearParameter();

    void addParameter(std::string parameter_marker, int value);
    void addParameter(std::string parameter_marker, float value);
    void addParameter(std::string parameter_marker, std::string value);

    void setMinTriangleArea(float area);
    void setMinTriangleAngle(float angle);


private:
    std::string softwarePath;

    struct Parameter_t{
        std::string marker;
        std::string value;
        Parameter_t* nextParameter;
    };

    struct Parameter_t *parameters;
};



#endif