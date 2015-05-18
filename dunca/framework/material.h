#if !defined SD_FW_MATERIAL_H
#define SD_FW_MATERIAL_H

#include "shader.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

class Material
{
public:

    Shader mProgram; // Transformation and material

    virtual bool setup();
    void bind();

    // Do nothing, just a way for subclass to retrieve uniforms
    virtual bool configUniforms();
    // Do nothing, just a way for subclass to update uniforms
    virtual bool updateUniforms();
    // Do nothing, just a way for subclass to provide another vertex attribute pos
    virtual GLuint vertexAttribPos() const;
    // Do nothing, just a way for subclass to provide another color attribute pos
    virtual GLuint colorAttribPos() const;

    // Do nothing, just a way for subclass to provide a more complex way of deciding
    virtual bool
    Material::hasColorAttribPos() const;
};

/*virtual*/ inline bool
Material::configUniforms()
{
    return true; // Do nothing, just a way for subclass to retrive uniforms
}

/*virtual*/ inline bool
Material::updateUniforms()
{
    return true; // Do nothing, just a way for subclass to update uniforms
}

/*virtual*/ inline GLuint
Material::vertexAttribPos() const
{
    return 0;
}

/*virtual*/ inline GLuint
Material::colorAttribPos() const
{
    return -1;
}

/*virtual*/ inline bool
Material::hasColorAttribPos() const
{
    return colorAttribPos() != -1;
}

#endif // #ifdef SD_FW_MATERIAL_H