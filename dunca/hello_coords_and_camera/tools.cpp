#include "tools.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


/*static*/ bool
Tools::readFile(std::string& code, const char* path)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    bool ok = true;
    try 
    {
        // Open files
        std::ifstream vShaderFile(path);
        std::stringstream vShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        // Convert stream into string
        code = vShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        ok = false;
    }
    return ok;
}
