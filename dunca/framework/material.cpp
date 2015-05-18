#include "material.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>


/*virtual*/ bool
Material::setup()
{
    return configUniforms();
}

/*virtual*/ void
Material::bind()
{
    mProgram.Use();

    // Update uniforms after program is set otherwise glUniform* calls won't work
    updateUniforms();
}