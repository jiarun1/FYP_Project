/***********************************************************
 * \file      : triangleCommand.cpp
 * \brief     : This file contained a class that used call the 
 *              command line for the software: Triangle
 *              
 * \copyright : Copyright Jiarun LIU
 * \author    : Jiarun LIU
 * \version   : V0.1
***********************************************************/
#include "triangleCommand.h"

#include "iostream"
#include "string"


triangleCommand::triangleCommand(std::string software_path):
    softwarePath(software_path)
{
    
}


triangleCommand::~triangleCommand()
{

}

void triangleCommand::clearParameter()
{
    struct Parameter_t* parameter_tmp = parameters;
    while (parameter_tmp != nullptr)
    {
        if(parameter_tmp->nextParameter != nullptr)
        {
            auto parameter_delete = parameter_tmp;
            parameter_tmp = parameter_tmp->nextParameter;
            delete parameter_delete;
        } else {
            delete parameter_tmp;
            break;;
        }
    }
    parameters = nullptr;
    return;
}

void triangleCommand::setParameter(std::string parameter_marker, int value)
{
    setParameter(parameter_marker,std::to_string(value));
}

void triangleCommand::setParameter(std::string parameter_marker, float value)
{
    setParameter(parameter_marker,std::to_string(value));
}

void triangleCommand::setParameter(std::string parameter_marker, std::string value)
{
    struct Parameter_t* parameter_tmp = parameters;
    if(parameter_tmp == nullptr){
        struct Parameter_t* parameter_new = new (struct Parameter_t);
        parameter_new->marker = parameter_marker;
        parameter_new->value = value;
        parameter_new->nextParameter = nullptr;
        parameters = parameter_new;
    } else {

        do {
            if(parameter_marker == parameter_tmp->marker){
                parameter_tmp->value = value;
                return;
            }
            parameter_tmp = parameter_tmp->nextParameter;
        }while(parameter_tmp->nextParameter != nullptr);

        struct Parameter_t* parameter_new = new (struct Parameter_t);
        parameter_new->marker = parameter_marker;
        parameter_new->value = value;
        parameter_new->nextParameter = nullptr;
        parameter_tmp->nextParameter = parameter_new;
    }
}



void triangleCommand::call(std::string poly_path)
{
    std::string command = softwarePath + " ";
    std::string parameter_data;

    parameter_data.clear();
    auto parameter_tmp = parameters;
    while (parameter_tmp != nullptr)
    {
        parameter_data.append(parameter_tmp->marker).append(parameter_tmp->value);

        if(parameter_tmp->nextParameter==nullptr)
        {
            break;
        }
        parameter_tmp = parameter_tmp->nextParameter;
    }
    
    if(parameter_data.size() != 0)
    {
        command.append("-").append(parameter_data).append(" ");
    }

    command.append(poly_path);

    std::cout << "Command: " << command << std::endl;

    system(command.c_str());
}
