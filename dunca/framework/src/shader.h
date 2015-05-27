#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
public:
    // Constructor generates the shader on the fly
    Shader();

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath,
        const GLchar* geometryPath = nullptr);

    bool
    load(const GLchar* vShaderCode, const GLchar * fShaderCode,
        const GLchar * gShaderCode = nullptr);

    // Uses the current shader
    void
    use();

    GLuint
    GetProgram() { return mProgram; };

private:
    GLuint mProgram;
};

#endif