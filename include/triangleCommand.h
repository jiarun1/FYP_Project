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

    /**
     * \defgroup dealing with the input files
     * @{
    */

    /// @brief clean the temperatory files generated by Triangle
    /// @param poly_path the .poly file which have been input to the file
    /// @param verification if the verfication of the process is needed
    /// @return if the delete procedure is successful
    bool cleanTmpFiles(std::string poly_path, bool verification = true);
    /**
     * @}
    */

    /**
     * \defgroup all parameter settings to the triangle
     * @{
    */
    
    void call(std::string poly_path);

    void clearParameter();

    void setParameter(std::string parameter_marker, int value);
    void setParameter(std::string parameter_marker, float value);
    void setParameter(std::string parameter_marker, double value);
    void setParameter(std::string parameter_marker, std::string value = "");

    void deleteParameter(std::string parameter_marker);

    void setMaxTriangleArea(float area);
    void setMinTriangleAngle(float angle);
    /// @brief set CDT
    void setConformingDelaunaryTriangle(bool enableCDT);
    /**
     * @}
    */

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