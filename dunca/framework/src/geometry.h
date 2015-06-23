#if !defined SD_FW_GEOMETRY_H
#define SD_FW_GEOMETRY_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <memory>

class Material;

class Geometry
{
    // We want as much data in here as we could in order to
    // minimizes switching of VBOs
    GLuint mVbo;
    GLuint mEbo;
    GLuint mVao;

    union {
        GLuint mCoordinatesCount;
        GLuint mIndicesCount;
    };

    std::shared_ptr<Material> mMaterial;

    bool mBackfaceCulled;

public:

    Geometry(std::shared_ptr<Material> material)
        : mVbo(-1)
        , mEbo(-1)
        , mCoordinatesCount(0) // mIndicesCount(0)
        , mMaterial(material)
        , mBackfaceCulled(true)
    {
    }

    Geometry()
        : mVbo(-1)
        , mEbo(-1)
        , mCoordinatesCount(0) // mIndicesCount(0)
        , mBackfaceCulled(true)
    {
    }

    ~Geometry()
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
        std::shared_ptr<Geometry> obj;
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

    void setup(const GeometryParameters& p,
        GeometryParamsExt* ext = nullptr) throw();
    void teardown();

    void draw();

    virtual void
    prepare() {};

    virtual void
    close() {};

    bool hasEbo() { return mEbo != -1; }

    bool
    isBackfaceCooled() { return mBackfaceCulled; };
    void
    setBackfaceCooled(bool enable) { mBackfaceCulled = enable; };

    std::shared_ptr<Material> getMaterial();
};

//******************************************************************************
inline std::shared_ptr<Material>
Geometry::getMaterial()
{
    return mMaterial;
}

#endif // #ifdef SD_FW_OBJECT_H