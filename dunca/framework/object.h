#if !defined SD_FW_OBJECT_H
#define SD_FW_OBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <memory>

class Material;

class Object
{
    // We want as much data in here as we could in order to
    // minimizes switching of VBOs
    GLuint mVbo;
    GLuint mEbo;
    GLuint mVao;
    // Not owned
    std::shared_ptr<Material> mMaterial; // Transformation and material

    union {
        GLuint mVerticesCount;
        GLuint mIndicesCount;
    };

public:

    Object()
        : mVbo(-1)
        , mEbo(-1)
        , mVerticesCount(0)
    {
    }
    ~Object()
    {
        teardown();
    }

    typedef enum
    {
        Vertex = 0,
        TextCoordinates,
        Color,
        AttribOrder_Count,
        NoAttrib
    } AttribOrder;

    struct GeometryParameters
    {
        GeometryParameters()
            : vertices(nullptr)
            , verticesCount(0)
            , indices(nullptr)
            , indicesCount(0)
            , obj(nullptr)
        {
            haveAttribs[Vertex] = true;
            haveAttribs[TextCoordinates] = false;
            haveAttribs[Color] = false;
        }

        const GLfloat* vertices;
        GLuint verticesCount;
        const GLuint* indices;
        GLuint indicesCount;
        bool haveAttribs[AttribOrder_Count];
        Object* obj;
        static const GLenum attribsCount[AttribOrder_Count];
        static const unsigned char defaultTypeSize;
    };

    struct GeometryParamsExt
    {
        GeometryParamsExt()
        {
            attribsOrder[0] = Vertex;
            attribsOrder[1] = NoAttrib;
            attribsOrder[2] = NoAttrib;
            attribsTypes[Vertex] = GL_FLOAT;
            attribsTypes[TextCoordinates] = GL_FLOAT;
            attribsTypes[Color] = GL_FLOAT;
            typeSize[Vertex] = sizeof(GLfloat);
            typeSize[TextCoordinates] = sizeof(GLfloat);
            typeSize[Color] = sizeof(GLfloat);
        }
        GLuint typeSize[AttribOrder_Count];
        AttribOrder attribsOrder[AttribOrder_Count];
        GLenum attribsTypes[AttribOrder_Count];
    };

    bool setup(const GeometryParameters& p, GeometryParamsExt* ext = nullptr);
    void teardown();

    void draw();

    bool hasEbo() { return mEbo != -1; }

    void setMaterial(std::shared_ptr<Material> material) { mMaterial = material; }; // Transformation and material
};

#endif // #ifdef SD_FW_OBJECT_H