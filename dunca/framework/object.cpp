#define GLEW_STATIC
#include "object.h"

#include "material.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>


const GLenum Object::GeometryParameters::attribsCount[AttribOrder_Count] = {3, 2, 3};
const unsigned char Object::GeometryParameters::defaultTypeSize = sizeof(GLfloat);

bool
Object::setup(const GeometryParameters& p, GeometryParamsExt* ext/*=nullptr*/)
{
    if(p.vertices != nullptr)
    {
        // Setup vertex buffer to keep the vertices
        glGenBuffers( 1, &mVbo );
        // Bind the newly created buffer to the array buffer target
        glBindBuffer( GL_ARRAY_BUFFER, mVbo );
        // Upload buffer to GPU in its provided location
        glBufferData( GL_ARRAY_BUFFER, p.verticesCount * sizeof(GLfloat),
            p.vertices, GL_STATIC_DRAW );
        mVerticesCount = p.verticesCount;
    }
    else if( p.obj != NULL )
    {
        mVbo = p.obj->mVbo;
        mVerticesCount = p.obj->mVerticesCount;
    }
    else
    {
        return false;
    }
    // Setup EBO if needed
    if(p.indices != NULL)
    {
        // Setup vertex buffer to keep the vertices
        glGenBuffers( 1, &mEbo );
        // Bind the newly created buffer to the array buffer target
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  mEbo );
        // Upload buffer to GPU in its provided location
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, p.indicesCount * sizeof(GLuint),
            p.indices, GL_STATIC_DRAW );
        mIndicesCount = p.indicesCount;
    }

    // Generate VAO to represent object data on GPU
    glGenVertexArrays(1, &mVao);
    // Start binding appropriate data to this VAO
    glBindVertexArray(mVao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        if(hasEbo())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        }
        GLuint strideSize = 0;
        // Setup position of attributes
        GLuint attribPos[AttribOrder_Count] = {0, -1, -1};
        bool haveAttribs[AttribOrder_Count];
        for(int i = 0; i < AttribOrder_Count; ++i)
        {
            haveAttribs[i] = p.haveAttribs[i];
        }
        // Setup attribute order
        GLvoid* attribOffset[AttribOrder_Count] = {0, 0, 0};
        bool eOn = (ext != nullptr);
        int addToOffset = 0;
        for(int i = 0; i < AttribOrder_Count; ++i)
        {
            int typeSize = GeometryParameters::defaultTypeSize; // Default
            int attrib = i;
            if(eOn)
            {
                if(ext->attribsOrder[i] != NoAttrib)
                    break;
                attrib = ext->attribsOrder[i];
                typeSize = ext->typeSize[attrib];
                haveAttribs[attrib] = typeSize > 0;
            }
            if(!haveAttribs[attrib])
                continue;
            addToOffset = GeometryParameters::attribsCount[attrib] * typeSize;
            attribOffset[attrib] = reinterpret_cast<GLvoid*>(strideSize);
            strideSize += addToOffset;
        }

        if(mMaterial != NULL)
        {
            attribPos[Vertex] = mMaterial->vertexAttribPos();

            haveAttribs[TextCoordinates] = haveAttribs[TextCoordinates]
                & mMaterial->hasTextCoordAttribPos();
            if(haveAttribs[TextCoordinates])
            {
                attribPos[TextCoordinates] = mMaterial->textCoordAttribPos();
            }

            haveAttribs[Color] = haveAttribs[Color]
                && mMaterial->hasColorAttribPos();
            if(haveAttribs[Color])
            {   
                attribPos[Color] = mMaterial->colorAttribPos();
            }
        }
        for(int i = 0; i < AttribOrder_Count; ++i)
        {
            int attrib = eOn ? ext->attribsOrder[i] : i;
            if(haveAttribs[attrib] && attribPos[attrib] != -1)
            {
                glVertexAttribPointer(attribPos[attrib], GeometryParameters::attribsCount[attrib],
                    eOn ? ext->attribsTypes[attrib] : GL_FLOAT, GL_FALSE, strideSize, attribOffset[attrib]);
                glEnableVertexAttribArray(attribPos[attrib]);
            }
        }
    }
    // End of binding VAO setup
    glBindVertexArray(0);

    return mMaterial != NULL ? mMaterial->configUniforms() : true;
}

void
Object::teardown()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers( 1, &mEbo );
    glDeleteBuffers( 1, &mVbo );

    mMaterial.reset();
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
    if(hasEbo())
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