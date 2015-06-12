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

    virtual void setup(std::shared_ptr<Texture> texture = std::shared_ptr<Texture>()) throw();
    void bind();
    virtual void prepare() {};

    virtual bool vertexShader(std::string& code) const throw() {return false;};
    virtual bool fragmentShader(std::string& code) const throw() {return false;};

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
    // Return model transformation matrix uniform location or -1 if it's the case
    virtual GLuint modelUniformLoc() const;
    virtual bool hasModelUniformLoc() const;
    // Return view transformation matrix uniform location or -1 if it's the case
    virtual GLuint viewUniformLoc() const;
    virtual bool hasViewUniformLoc() const;

    // Do nothing, just a way for subclass to provide a more complex way of deciding
    virtual bool
    hasColorAttribPos() const;
    // Do nothing, just a way for subclass to provide a more complex way of deciding
    virtual bool
    hasTextCoordAttribPos() const;
};

//******************************************************************************
/*virtual*/ inline bool
Material::configUniforms()
{
    return true; // Do nothing, just a way for subclass to retrive uniforms
}

//******************************************************************************
/*virtual*/ inline bool
Material::updateUniforms()
{
    return true; // Do nothing, just a way for subclass to update uniforms
}

//******************************************************************************
/*virtual*/ inline GLuint
Material::vertexAttribPos() const
{
    return 0;
}

//******************************************************************************
/*virtual*/ inline GLuint
Material::colorAttribPos() const
{
    return -1;
}

//******************************************************************************
/*virtual*/ inline bool
Material::hasColorAttribPos() const
{
    return colorAttribPos() != -1;
}

//******************************************************************************
/*virtual*/ inline GLuint
Material::textCoordAttribPos() const
{
    return -1;
}

//******************************************************************************
/*virtual*/ inline GLuint
Material::modelUniformLoc() const
{
    return -1;
}

//******************************************************************************
/*virtual*/ inline bool
Material::hasModelUniformLoc() const
{
    return modelUniformLoc() != -1;
}

//******************************************************************************
/*virtual*/ inline GLuint
Material::viewUniformLoc() const
{
    return -1;
}

//******************************************************************************
/*virtual*/ inline bool
Material::hasViewUniformLoc() const
{
    return viewUniformLoc() != -1;
}

//******************************************************************************
/*virtual*/ inline bool
Material::hasTextCoordAttribPos() const
{
    return textCoordAttribPos() != -1;
}


#endif // #ifdef SD_FW_MATERIAL_H