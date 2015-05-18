#define GLEW_STATIC
#include "object.h"

#include "material.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>


bool
Object::setup(const GLfloat* vertices, GLuint verticesCount,
        const GLuint* indices/*= NULL*/, GLuint indicesCount/*= 0*/,
        GLuint colorTypeSize/*= 0*/, Object* obj/*= NULL*/ )
{
    if(vertices != NULL)
    {
        // Setup vertex buffer to keep the vertices
        glGenBuffers( 1, &mVbo );
        // Bind the newly created buffer to the array buffer target
        glBindBuffer( GL_ARRAY_BUFFER, mVbo );
        // Upload buffer to GPU in its provided location
        glBufferData( GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW );
        mVerticesCount = verticesCount;
    }
    else if( obj != NULL )
    {
        mVbo = obj->mVbo;
        mVerticesCount = obj->mVerticesCount;
    }
    else
    {
        return false;
    }
    // Setup EBO if needed
    if(indices != NULL)
    {
        // Setup vertex buffer to keep the vertices
        glGenBuffers( 1, &mEbo );
        // Bind the newly created buffer to the array buffer target
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  mEbo );
        // Upload buffer to GPU in its provided location
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint),
            indices, GL_STATIC_DRAW );
        mIndicesCount = indicesCount;
        mHasEbo = true;
    }
    else
    {
        mHasEbo = false;
    }

    // Generate VAO to represent object data on GPU
    glGenVertexArrays(1, &mVao);
    // Start binding appropriate data to this VAO
    glBindVertexArray(mVao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        if(mHasEbo)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        }
        // Setup properties vertex data attribute
        GLuint vertexAttribPos = 0;
        GLuint colorAttribPos = -1;
        GLuint strideSize = 3 * sizeof(GLfloat);
        bool havePerVertexAttribColor = colorTypeSize > 0;
        if(mMaterial != NULL)
        {
            vertexAttribPos = mMaterial->vertexAttribPos();
            if(havePerVertexAttribColor)
            {
                strideSize += colorTypeSize;
            }
            if(mMaterial->hasColorAttribPos())
            {
                colorAttribPos = mMaterial->colorAttribPos();
            }
        }
        glVertexAttribPointer(vertexAttribPos, 3, GL_FLOAT, GL_FALSE,
            strideSize, static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(vertexAttribPos);
        if(havePerVertexAttribColor && colorAttribPos != -1)
        {
            glVertexAttribPointer(colorAttribPos, 3, GL_FLOAT, GL_FALSE,
                strideSize, reinterpret_cast<GLvoid*>(colorTypeSize));
            glEnableVertexAttribArray(colorAttribPos);
        }
    }
    // End of binding VAO setup
    glBindVertexArray(0);

    return mMaterial != NULL ? mMaterial->configUniforms() : true;
}

void
Object::draw()
{
    if(mMaterial != NULL)
    {
        mMaterial->bind();
    }

    glBindVertexArray(mVao);
    // Draw triangles
    if(mHasEbo)
    {
        glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, mVerticesCount);
    }
    // Unbind this VAO
    glBindVertexArray(0);
}