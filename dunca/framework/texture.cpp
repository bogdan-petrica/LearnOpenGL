#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    GLuint Program;

    // Constructor generates the shader on the fly
    Shader()
    {
    }

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr)
    {
        load(vertexPath, fragmentPath, geometryPath);
    }

    bool
    load(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        bool ok = true;
        try 
        {
            // Open files
            std::ifstream vShaderFile(vertexPath);
            std::ifstream fShaderFile(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
			// If geometry shader path is present, also load a geometry shader
			if(geometryPath != nullptr)
			{
                std::ifstream gShaderFile(geometryPath);
                std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
        }
        catch (std::exception e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            ok = false;
        }
        if(ok)
        {
            const GLchar* vShaderCode = vertexCode.c_str();
            const GLchar * fShaderCode = fragmentCode.c_str();
            // 2. Compile shaders
            GLuint vertex, fragment;
            // Vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            ok = checkCompileErrors(vertex, "VERTEX");
            if( ok )
            {
                // Fragment Shader
                fragment = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragment, 1, &fShaderCode, NULL);
                glCompileShader(fragment);
		        ok = checkCompileErrors(fragment, "FRAGMENT");
		        // If geometry shader is given, compile geometry shader
		        GLuint geometry;
		        if(ok && geometryPath != nullptr)
		        {
			        const GLchar * gShaderCode = geometryCode.c_str();
			        geometry = glCreateShader(GL_GEOMETRY_SHADER);
			        glShaderSource(geometry, 1, &gShaderCode, NULL);
			        glCompileShader(geometry);
			        checkCompileErrors(geometry, "GEOMETRY");
		        }
                if(ok)
                {
                    // Shader Program
                    this->Program = glCreateProgram();
                    glAttachShader(this->Program, vertex);
                    glAttachShader(this->Program, fragment);
		            if(geometryPath != nullptr)
			            glAttachShader(this->Program, geometry);
                    glLinkProgram(this->Program);
                    checkCompileErrors(this->Program, "PROGRAM");
                    // Delete the shaders as they're linked into our program now and no longer necessary
                    glDeleteShader(vertex);
                    glDeleteShader(fragment);
		            if(geometryPath != nullptr)
			            glDeleteShader(geometry);
                }
            }
        }
        return ok;
    }
    // Uses the current shader
    void Use() { glUseProgram(this->Program); }

private:
    bool checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if(type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "| ERROR::::SHADER-COMPILATION-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "| ERROR::::PROGRAM-LINKING-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
			}
		}
        return success == GL_TRUE;
	}
};

#endif