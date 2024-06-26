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
    parameters = nullptr;
}


triangleCommand::~triangleCommand()
{

}

bool triangleCommand::cleanTmpFiles(std::string poly_path, bool verification)
{
    auto index_extention = poly_path.find_last_of(".");
    std::string filePath_Name = poly_path.substr(0, index_extention);
    std::string node_file = filePath_Name + ".1.node";
    std::string ele_file = filePath_Name + ".1.ele";
    std::string poly_file = filePath_Name + ".1.poly";


    if(verification == true)
    {
        std::cout << "Please verify the following files would be deleted (y for yes)" << std::endl;
        std::cout << "Node File: " << node_file << std::endl;
        std::cout << "Ele File : " << ele_file << std::endl;
        std::cout << "Poly File: " << poly_file << std::endl;

        char input;
        std::cin >> input;
        if(input != 'y')
        {
            return false;
        }
    }

    std::remove(node_file.c_str());
    std::remove(ele_file.c_str());
    std::remove(poly_file.c_str());
    return true;
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

void triangleCommand::setParameter(std::string parameter_marker, double value)
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

        while(1) {
            if(parameter_marker == parameter_tmp->marker){
                parameter_tmp->value = value;
                return;
            }
            if(parameter_tmp->nextParameter == nullptr)
            {
                break;
            }
            parameter_tmp = parameter_tmp->nextParameter;
        }

        struct Parameter_t* parameter_new = new (struct Parameter_t);
        parameter_new->marker = parameter_marker;
        parameter_new->value = value;
        parameter_new->nextParameter = nullptr;
        parameter_tmp->nextParameter = parameter_new;
    }
}

void triangleCommand::deleteParameter(std::string parameter_marker)
{
    auto current_parameter = parameters;
    auto previous_parameter = parameters;
    if(current_parameter == nullptr){
        return;
    } else if (current_parameter->marker == parameter_marker) //< used to delete the first parameter
    {
        if(current_parameter->nextParameter == nullptr){
            parameters = nullptr;
        } else {
            parameters = current_parameter->nextParameter;
        }
        delete current_parameter;
    } else {
        while (1)
        {
            previous_parameter = current_parameter;
            current_parameter = current_parameter->nextParameter;
            if(parameter_marker == current_parameter->marker){
                previous_parameter->nextParameter = current_parameter->nextParameter;
                delete current_parameter;
                return;
            }
            if(current_parameter->nextParameter == nullptr)
            {
                break;
            }
        }
        
        
        


    }


}



void triangleCommand::call(std::string poly_path)
{
    std::string command = softwarePath + " ";
    
    // std::string parameter_data;
    // parameter_data.clear();
    // auto parameter_tmp = parameters;
    // while (parameter_tmp != nullptr)
    // {
    //     parameter_data.append(parameter_tmp->marker).append(" ").append(parameter_tmp->value);

    //     if(parameter_tmp->nextParameter==nullptr)
    //     {
    //         break;
    //     }
    //     parameter_tmp = parameter_tmp->nextParameter;
    // }
    
    // if(parameter_data.size() != 0)
    // {
    //     command.append("-").append(parameter_data).append(" ");
    // }

    auto parameter_tmp = parameters;
    while (parameter_tmp != nullptr)
    {
        command.append("-").append(parameter_tmp->marker).append(parameter_tmp->value).append(" ");

        if(parameter_tmp->nextParameter==nullptr)
        {
            break;
        }
        parameter_tmp = parameter_tmp->nextParameter;
        
    }
    


    command.append(poly_path);

    std::cout << "Command: " << command << std::endl;

    system(command.c_str());
}

void triangleCommand::setMaxTriangleArea(float area)
{
    if(area == 0)
    {
        deleteParameter("a");
        return;
    }
    setParameter("a", area);
}

void triangleCommand::setMinTriangleAngle(float angle)
{
    if(angle == 0)
    {
        // deleteParameter("q");
        return;
    }
    setParameter("q", angle);
}


void triangleCommand::setConformingDelaunaryTriangle(bool enableCDT)
{
    if(enableCDT == true)
    {
        setParameter("D");
    }
}