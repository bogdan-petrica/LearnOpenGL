#if !defined SD_FW_OBJECT_H
#define SD_FW_OBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

class Material;

class Object
{
    // We want as much data in here as we could in order to
    // minimizes switching of VBOs
    GLuint mVbo;
    GLuint mVerticesCount;
    GLuint mEbo;
    GLuint mIndicesCount;
    bool mHasEbo;

public:

    Material* mMaterial; // Transformation and material
    GLuint mVao;

    bool setup(const GLfloat* vertices, GLuint verticesCount,
        const GLuint* indices = NULL, GLuint indicesCount = 0,
        GLuint colorTypeSize = 0, Object* obj = NULL );

    void draw();
};

#endif // #ifdef SD_FW_OBJECT_H