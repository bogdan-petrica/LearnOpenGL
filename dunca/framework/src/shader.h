#ifndef SD_FW_SHADER_H
#define SD_FW_SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
public:
    // Constructor generates the shader on the fly
    Shader();

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath,
        const GLchar* geometryPath = nullptr);

    void
    load(const GLchar* vShaderCode, const GLchar * fShaderCode,
        const GLchar * gShaderCode = nullptr)  throw();

    // Uses the current shader
    void
    use();

    GLuint
    GetProgram() { return mProgram; };

private:
    GLuint mProgram;
};

#endif