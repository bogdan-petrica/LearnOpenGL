

#include "GLProgram.h"

#include <fstream>

namespace LGL {

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    /// GLProgram
    GLuint GLProgram::LoadShader(GLenum shaderType, const char * path)
    {
        std::ifstream input(path);
        if (!input.is_open())
            throw GLException("Failed to load shader from file");

        GLchar source[4096];
        input.read(source, sizeof(source)-1);
        if (!input.eof())
            throw GLException("Buffer capacity exceeded when reading shader content");

        source[input.gcount()] = 0;

        GLuint shaderId;
        shaderId = glCreateShader(shaderType);
        GLchar * arr[1] = { &source[0] };
        glShaderSource(shaderId, 1, arr, NULL);
        glCompileShader(shaderId);

        GLint success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar msg[512];
            glGetShaderInfoLog(shaderId, 512, NULL, msg);
            throw GLException(msg);
        }

        return shaderId;
    }

}

