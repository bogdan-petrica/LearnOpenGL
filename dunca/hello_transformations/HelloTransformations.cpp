#define GLEW_STATIC

#include "tools.h"

#include "object.h"
#include "shader.h"
#include "material.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <cassert>
#include <stdio.h>
#include <iostream>

class KeyCallback
{
public:
    virtual void statusChanged(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
};

std::vector<KeyCallback*> gKeyCallbacks;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    for (int i = 0; i < gKeyCallbacks.size(); ++i)
    {
        assert(gKeyCallbacks[i] != nullptr);
        gKeyCallbacks[i]->statusChanged(window, key, scancode, action, mode);
    }

    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

const GLfloat bkColor[3] = { 0.2f, 0.3f, 0.3f };

class ColorMaterial: public Material
{
public:
    bool vertexShader(std::string& code) const { return Tools::readFile(code, vertexShaderSz); };
    bool fragmentShader(std::string& code) const { return Tools::readFile(code, greenFragmentShaderSz); };

    bool
    configUniforms()
    {
        return true;
    }

    bool 
    updateUniforms()
    {
        return true;
    }
    
    GLuint vertexAttribPos() const { return 0; }; // Bind vertex data to first attribute "layout (location = 0)"
    GLuint colorAttribPos() const { return 1; }; // Bind color data to the second attribute "layout (location = 1)"

private:
    // Inline shaders source code
    static const char* vertexShaderSz;
    static const char* greenFragmentShaderSz;
};

const char* ColorMaterial::vertexShaderSz ="../../../dunca/hello_transformations/green.vs";
const char* ColorMaterial::greenFragmentShaderSz = "../../../dunca/hello_transformations/green.frag";

class TextureMaterial: public Material
{
public:
    TextureMaterial(const glm::mat4* transform)
        : mTransformation(transform)
    {
    }

    bool vertexShader(std::string& code) const
    {
        return Tools::readFile(code, scaleTextureVertexShaderSz);
    };
    bool fragmentShader(std::string& code) const
    {
        return Tools::readFile(code, textureFragmentShaderSz);
    };

    bool
    configUniforms()
    {
        mUniformLocation = glGetUniformLocation(mProgram.GetProgram(), "globalArg");
        mTransformUniform = glGetUniformLocation(mProgram.GetProgram(), "objectTransform");
        mCrateTextLoc = glGetUniformLocation(mProgram.GetProgram(), "crateTextureUnit");
        mSmileTextLoc = glGetUniformLocation(mProgram.GetProgram(), "smileTextureUnit");
        return mUniformLocation != -1;
    }

    bool 
    updateUniforms()
    {
        glUniform1f(mUniformLocation, static_cast<GLfloat>( abs(sin(glfwGetTime())) ));
        assert(mTransformation != nullptr);
        glUniformMatrix4fv(mTransformUniform, 1, GL_FALSE, glm::value_ptr(*mTransformation));
        glUniform1i(mCrateTextLoc, 0);
        glUniform1i(mSmileTextLoc, 1);
        return true; // Do nothing, just a way for subclass to update uniforms
    }

    GLuint vertexAttribPos() const { return 0; }; // Bind vertex data to first attribute "layout (location = 0)"
    GLuint colorAttribPos() const { return 1; }; // Bind color data to the second attribute "layout (location = 1)"
    GLuint textCoordAttribPos() const { return 2; }; // Bind texture data to the third attribute "layout (location = 2)"

private:
    const glm::mat4* mTransformation;
    GLuint mUniformLocation;
    GLuint mTransformUniform;
    GLuint mCrateTextLoc;
    GLuint mSmileTextLoc;

    // Inline shaders source code
    static const char* scaleTextureVertexShaderSz;
    static const char* textureFragmentShaderSz;
};

const char* TextureMaterial::scaleTextureVertexShaderSz ="../../../dunca/hello_transformations/texture.vs";
const char* TextureMaterial::textureFragmentShaderSz = "../../../dunca/hello_transformations/texture.frag";

class SceneData: public KeyCallback
{
public:
    void statusChanged(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if(action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            bool done = true;
            if(key == GLFW_KEY_UP || key == GLFW_KEY_DOWN)
                mTranslateX += key == GLFW_KEY_UP ? 0.1f : -0.1f;
            else if(key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT)
                mTranslateY += key == GLFW_KEY_LEFT ? -0.1f: 0.1f;
            else if(key == GLFW_KEY_G || key == GLFW_KEY_S)
                mScale += key == GLFW_KEY_G ? 0.1f : -0.1f;
            else if(key == GLFW_KEY_R || key == GLFW_KEY_L)
                mRotate += (glm::pi<float>()/10.0f) * (key == GLFW_KEY_L ? 1.0f : -1.0f);
            else if(key == GLFW_KEY_Q)
            {
                if(mModifyUniformTransform)
                    mUniformTransform = glm::mat4();
                else
                    mGlobalTransform = glm::mat4();
            }
            else if(key == GLFW_KEY_F)
            {
                mNearestFiltering = !mNearestFiltering;
            }
            else if(key == GLFW_KEY_U)
            {
                mModifyUniformTransform = !mModifyUniformTransform;
            }
            else
                done = false;
            if(done)
            {
                if(mModifyUniformTransform)
                {
                    mUniformTransform = updateTransform(mUniformTransform);
                }
                else
                {
                    mGlobalTransform = updateTransform(mGlobalTransform);
                    tearDown();
                    setup();
                }
            }
        }
    }

    SceneData()
        : mTranslateX(0)
        , mTranslateY(0)
        , mScale(1.0f)
        , mRotate(0.0f)
        , mNearestFiltering(true)
        , mModifyUniformTransform(true)
    {
    }

    bool
    setup()
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
            mNearestFiltering ? GL_NEAREST : GL_LINEAR);
        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const static float KRadius = 0.9f;
        struct GeometryData 
        {
            glm::vec3 vertex;
            glm::vec2 textCoords;
            glm::vec3 colorCoords;
        };
        GeometryData allVertices[] = {
            {glm::vec3(-KRadius, -KRadius, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},  // bottom-left
            {glm::vec3(KRadius, -KRadius, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},   // bottom-right
            {glm::vec3(KRadius,  KRadius, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)},   // top-right
            {glm::vec3(0.0f,  KRadius, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec3(0.5f, 1.0f, 0.5f)},      // top-center
            {glm::vec3(-KRadius,  KRadius, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f)}   // top-left
        };
        
        // Visually testing matrix transformation for one point
        glm::mat4 transTrianglePoint;
        transTrianglePoint = glm::translate(transTrianglePoint, glm::vec3(mTranslateY, mTranslateX, 0.0f));
        allVertices[3].vertex = glm::vec3(transTrianglePoint * glm::vec4(allVertices[3].vertex, 1.0f));

        for (int i = 0; i < sizeof(allVertices)/sizeof(allVertices[0]); ++i)
        {
            allVertices[i].vertex = glm::vec3(mGlobalTransform * glm::vec4(allVertices[i].vertex, 1.0f));
        }

        std::shared_ptr<Texture> triangleTextures = std::make_shared<Texture>();
        bool ok = triangleTextures->loadFromFile(mCrateTexturePath);
        ok = ok && triangleTextures->loadFromFile(mSmileTexturePath, true);
        std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>(
            &mUniformTransform);
        // Set program before setup where uniforms location is retrieved
        ok = ok && textureMaterial->setup(triangleTextures);
        if(ok)
        {
            // Set program before setup where uniforms location is retrieved
            triangle.setMaterial(textureMaterial);

            std::shared_ptr<ColorMaterial> colorMaterial = std::make_shared<ColorMaterial>();
            ok = colorMaterial->setup();
            if(ok) rectangle.setMaterial(colorMaterial);
        }

        {
            // Counterclockwise to look at the front-face ( screen space )
            const GLuint triangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/,
                3/*top-center*/ };
            Object::GeometryParameters params;
            params.vertices = reinterpret_cast<GLfloat*>(allVertices);
            params.verticesCount = sizeof(allVertices)/sizeof(allVertices[0].vertex.x);
            params.indices = triangleIndices;
            params.indicesCount = sizeof(triangleIndices)/sizeof(triangleIndices[0]);
            params.haveAttribs[Object::Color] = true;
            params.haveAttribs[Object::TextCoordinates] = true;
            ok = ok && triangle.setup(params);
        }
        {
            const GLuint rectangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/,
                2/*top-right*/, 2/*top-right*/, 4/*top-left*/, 0/*bottom-left*/ };
            Object::GeometryParameters params;
            params.indices = rectangleIndices;
            params.indicesCount = sizeof(rectangleIndices)/sizeof(rectangleIndices[0]);
            params.obj = &triangle; // We reuse the same vertex buffer that triangle uses
            params.haveAttribs[Object::Color] = true;
            params.haveAttribs[Object::TextCoordinates] = true;
            ok = rectangle.setup(params);
        }

        return ok;
    }

    void
    tearDown()
    {
        rectangle.teardown();
        triangle.teardown();
    }

    void
    drawFrame()
    {
        // Draw background to cover the previous frame
        glClearColor( bkColor[0], bkColor[1], bkColor[2], 1.0f );
        glClear(GL_COLOR_BUFFER_BIT);

        // Every 100 frames switch to drawing rectangle and vice versa
        rectangle.draw();
        // Draw the freaking triangle
        triangle.draw();
    }

    glm::mat4
    updateTransform(const glm::mat4& trans)
    {
        glm::mat4 result(glm::rotate(trans, mRotate, glm::vec3(0.0f, 0.0f, 1.0f)));
        result = glm::scale(result, glm::vec3(mScale, mScale, mScale));
        mRotate = 0;
        mScale = 1;
        return result;
    }

private:
    Object triangle;
    Object rectangle;
    float mTranslateX;
    float mTranslateY;
    float mScale;
    float mRotate;
    bool mNearestFiltering;
    bool mModifyUniformTransform;
    glm::mat4 mGlobalTransform;
    glm::mat4 mUniformTransform;


    static const char* mCrateTexturePath;
    static const char* mSmileTexturePath;
};

const char* SceneData::mCrateTexturePath = "../../../dunca/res/container.jpg";
const char* SceneData::mSmileTexturePath = "../../../dunca/res/awesome_face.png";

int main()
{
    GLFWwindow* window;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 600, "Hello Shaders (dunca)", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        printf( "Failed to create GLFW window\n" );
        glfwTerminate();
        return -1;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf( "Failed to initialize GLEW\n" );
        return -1;
    }

    glViewport( 0, 0, 800, 600 );

    glfwSetKeyCallback( window, &key_callback );

    // Setup scene
    SceneData scene;
    bool ok = scene.setup();

    gKeyCallbacks.push_back(&scene);

    while(ok && !glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Draw scene
        scene.drawFrame();

        glfwSwapBuffers(window);
    }

    // Cleanup scene
    scene.tearDown();

    glfwTerminate();

    return ok ? 0 : -1;
}
