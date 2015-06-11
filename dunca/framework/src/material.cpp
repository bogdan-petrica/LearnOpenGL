#include "material.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>


/*virtual*/ void
Material::setup(std::shared_ptr<Texture> texture/*=std::shared_ptr<Texture>()*/) throw()
{
    std::string vs;
    std::string fs;
    vertexShader(vs);
    fragmentShader(fs);
    if(!vs.empty() && !fs.empty())
    {
        mProgram.load(vs.c_str(), fs.c_str());
    }

    configUniforms();
    mTexture = texture;
}

/*virtual*/ void
Material::bind()
{
    if(mTexture != nullptr)
    {
        mTexture->bind();
    }
    mProgram.use();

    // Update uniforms after program is set otherwise glUniform* calls won't work
    updateUniforms();
}