#if !defined SD_FW_MATERIAL_H
#define SD_FW_MATERIAL_H

#include "shader.h"
#include "texture.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <memory>

class Material
{
public:

    Material()
        : mTexture(nullptr)
    {
    };

    Shader mProgram; // Program object
    std::shared_ptr<Texture> mTexture; // Program object

    virtual bool setup(std::shared_ptr<Texture> texture = std::shared_ptr<Texture>());
    void bind();

    virtual bool vertexShader(std::string& code) const {return false;};
    virtual bool fragmentShader(std::string& code) const {return false;};

    // Do nothing, just a way for subclass to retrieve uniforms
    virtual bool configUniforms();
    // Do nothing, just a way for subclass to update uniforms
    virtual bool updateUniforms();
    // Do nothing, just a way for subclass to provide another vertex attribute pos
    virtual GLuint vertexAttribPos() const;
    // Do nothing, just a way for subclass to provide another color attribute pos
    virtual GLuint colorAttribPos() const;
    // Do nothing, just a way for subclass to provide another texture coordinate attribute pos
    virtual GLuint textCoordAttribPos() const;

    // Do nothing, just a way for subclass to provide a more complex way of deciding
    virtual bool
    hasColorAttribPos() const;
    // Do nothing, just a way for subclass to provide a more complex way of deciding
    virtual bool
    hasTextCoordAttribPos() const;
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

/*virtual*/ inline GLuint
Material::textCoordAttribPos() const
{
    return -1;
}

/*virtual*/ inline bool
Material::hasTextCoordAttribPos() const
{
    return textCoordAttribPos() != -1;
}

#endif // #ifdef SD_FW_MATERIAL_H