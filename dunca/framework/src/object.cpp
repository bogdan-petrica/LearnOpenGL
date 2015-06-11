#include "object.h"

#include "material.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <iostream>

static const glm::vec3 gScaleVector(1.0f, 1.0f, 1.0f);
static const glm::vec3 gTranslateVector(0.0f, 0.0f, 0.0f);
static const glm::vec3 gRotationVector(0.0f, 0.0f, 0.0f);

//******************************************************************************
Object::Object()
    : rotate(gRotationVector)
    , translate(gTranslateVector)
    , scale(gScaleVector)
{
    
}

//******************************************************************************
/*virtual*/ void
Object::computeModelMatrix(glm::mat4& m)
{
    if(scale != gScaleVector)
        m = glm::scale(m, scale);

    for (int i = 0; i < rotate.length(); ++i)
    {
        if(rotate[i] != 0.0f)
        {
            glm::vec3 axis;
            axis[i] = 1.0f;
            m = glm::rotate(m, rotate[i], axis);
        }
    }

    if(translate != gTranslateVector)
        m = glm::translate(m, translate);
}
