#include "material.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>


/*virtual*/ bool
Material::setup(std::shared_ptr<Texture> texture/*=std::shared_ptr<Texture>()*/)
{
    std::string vs;
    std::string fs;
    bool ok = vertexShader(vs);
    ok = ok && fragmentShader(fs);
    if(ok && !vs.empty() && !fs.empty())
    {
        ok = mProgram.load(vs.c_str(), fs.c_str());
    }

    if(ok)
    {
        ok = configUniforms();
        mTexture = texture;
    }

    return ok;
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