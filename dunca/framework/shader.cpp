#include "shader.h"

#include <iostream>

// Forward declaration
bool
checkCompileErrors(GLuint shader, std::string type);

Shader::Shader()
{
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath,
    const GLchar* geometryPath)
{
    load(vertexPath, fragmentPath, geometryPath);
}

bool
Shader::load(const GLchar* vShaderCode, const GLchar * fShaderCode,
    const GLchar * gShaderCode)
{
    bool ok = true;
    if(ok)
    {
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
            if(ok && gShaderCode != nullptr)
            {
                    
                geometry = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometry, 1, &gShaderCode, NULL);
                glCompileShader(geometry);
                checkCompileErrors(geometry, "GEOMETRY");
            }
            if(ok)
            {
                // Shader Program
                this->mProgram = glCreateProgram();
                glAttachShader(this->mProgram, vertex);
                glAttachShader(this->mProgram, fragment);
                if(gShaderCode != nullptr)
                    glAttachShader(this->mProgram, geometry);
                glLinkProgram(this->mProgram);
                checkCompileErrors(this->mProgram, "PROGRAM");
                // Delete the shaders as they're linked into our program now and no longer necessary
                glDeleteShader(vertex);
                glDeleteShader(fragment);
                if(gShaderCode != nullptr)
                    glDeleteShader(geometry);
            }
        }
    }
    return ok;
}

void
Shader::use()
{
    glUseProgram(this->mProgram);
}

bool
checkCompileErrors(GLuint shader, std::string type)
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