#define GLEW_STATIC

#include "tools.h"

#include "geometry.h"
#include "shader.h"
#include "material.h"
#include "window.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "item.h"
#include "object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <time.h>

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

const char* ColorMaterial::vertexShaderSz ="../../../dunca/hello_coords_and_camera/green.vs";
const char* ColorMaterial::greenFragmentShaderSz = "../../../dunca/hello_coords_and_camera/green.frag";

class TextureMaterial: public Material
{
public:
    TextureMaterial()
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
        mVariationUniformLoc = glGetUniformLocation(mProgram.GetProgram(), "uGlobalVariationVal");
        mModelUniformLoc = glGetUniformLocation(mProgram.GetProgram(), "uModelMatrix");
        mViewUniformLoc = glGetUniformLocation(mProgram.GetProgram(), "uViewMatrix");
        mCrateTextLoc = glGetUniformLocation(mProgram.GetProgram(), "crateTextureUnit");
        mSmileTextLoc = glGetUniformLocation(mProgram.GetProgram(), "smileTextureUnit");
        return mVariationUniformLoc != -1 && mCrateTextLoc != -1 && mSmileTextLoc != -1;
    }

    bool 
    updateUniforms()
    {
        glUniform1f(mVariationUniformLoc, static_cast<GLfloat>( abs(sin(glfwGetTime())) ));
        glUniform1i(mCrateTextLoc, 0);
        glUniform1i(mSmileTextLoc, 1);
        return true; // Do nothing, just a way for subclass to update uniforms
    }

    GLuint
    modelUniformLoc() const { return mModelUniformLoc; }
    GLuint
    viewUniformLoc() const { return mViewUniformLoc; }

    GLuint vertexAttribPos() const { return 0; }; // Bind vertex data to first attribute "layout (location = 0)"
    GLuint colorAttribPos() const { return 1; }; // Bind color data to the second attribute "layout (location = 1)"
    GLuint textCoordAttribPos() const { return 2; }; // Bind texture data to the third attribute "layout (location = 2)"

private:
    GLuint mVariationUniformLoc;

    GLuint mModelUniformLoc;
    GLuint mViewUniformLoc;

    GLuint mCrateTextLoc;
    GLuint mSmileTextLoc;

    // Inline shaders source code
    static const char* scaleTextureVertexShaderSz;
    static const char* textureFragmentShaderSz;
};

class RectangleGeometry: public Geometry
{
public:
    RectangleGeometry(std::shared_ptr<Material> material)
        : Geometry(material)
    {
    }

    void
    prepare()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void
    close()
    {
        glEnable(GL_DEPTH_TEST);
    }
};

const char* TextureMaterial::scaleTextureVertexShaderSz ="../../../dunca/hello_coords_and_camera/texture.vs";
const char* TextureMaterial::textureFragmentShaderSz = "../../../dunca/hello_coords_and_camera/texture.frag";

class SceneData
{
public:
    SceneData()
        : mTriangleObj(new Object())
        , mCubeObj(new Object())
    {
    }

    void
    setup(Scene& scene) throw()
    {
        const static float KRadius = 0.9f;
        struct GeometryData 
        {
            glm::vec3 vertex;
            glm::vec2 textCoords;
            glm::vec3 colorCoords;
        };
        GeometryData allVertices[] = {
            {glm::vec3(-KRadius, -KRadius, -KRadius), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},  // bottom-left-back
            {glm::vec3(KRadius, -KRadius, -KRadius), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},   // bottom-right-back
            {glm::vec3(KRadius,  KRadius, -KRadius), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)},   // top-right-back
            {glm::vec3(0.0f,  KRadius, -KRadius), glm::vec2(0.5f, 1.0f), glm::vec3(0.5f, 1.0f, 0.5f)},      // top-center-back
            {glm::vec3(-KRadius,  KRadius, -KRadius), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f)},   // top-left-back
            // Second
            {glm::vec3(-KRadius, -KRadius, KRadius), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},  // bottom-left-front
            {glm::vec3(KRadius, -KRadius, KRadius), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},   // bottom-right-front
            {glm::vec3(KRadius,  KRadius, KRadius), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)},   // top-right-front
            {glm::vec3(-KRadius,  KRadius, KRadius), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f)}   // top-left-front
        };

        std::shared_ptr<Texture> triangleTextures = std::make_shared<Texture>();
        triangleTextures->loadFromFile(mCrateTexturePath);
        triangleTextures->loadFromFile(mSmileTexturePath, true);
        std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();
        textureMaterial->setup(triangleTextures);
        std::shared_ptr<ColorMaterial> colorMaterial = std::make_shared<ColorMaterial>();
        colorMaterial->setup();
        // Set program before setup where uniforms location is retrieved
        std::shared_ptr<Geometry> triangle = std::make_shared<Geometry>(textureMaterial);
        std::shared_ptr<RectangleGeometry> rectangle = std::make_shared<RectangleGeometry>(colorMaterial);
        std::shared_ptr<Geometry> cube = std::make_shared<Geometry>(textureMaterial);
        Geometry::GeometryParameters params;
        {
            // Counterclockwise to look at the front-face ( screen space )
            const GLuint triangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/, 3/*top-center*/ };
            params.vertices = reinterpret_cast<GLfloat*>(allVertices);
            params.verticesCount = sizeof(allVertices)/sizeof(allVertices[0].vertex);
            params.indices = triangleIndices;
            params.indicesCount = sizeof(triangleIndices)/sizeof(triangleIndices[0]);
            params.haveAttribs[Geometry::Color] = true;
            params.haveAttribs[Geometry::TextCoordinates] = true;
            triangle->setBackfaceCooled(false);
            triangle->setup(params);
        }
        {
            const GLuint rectangleIndices[] = {
                0/*bottom-left*/, 1/*bottom-right*/, 2/*top-right*/,
                2/*top-right*/, 4/*top-left*/, 0/*bottom-left*/ };
            params.indices = rectangleIndices;
            params.indicesCount = sizeof(rectangleIndices)/sizeof(rectangleIndices[0]);
            params.obj = triangle; // We reuse the same vertex buffer that triangle uses
            rectangle->setup(params);
        }
        {
            // Specify vertices in the clock-inverse order
            const GLuint cubeIndices[] = {
                2, 1, 0, 0, 4, 2, /*back face*/
                0, 5, 8, 8, 4, 0, /*left face*/
                8, 7, 2, 2, 4, 8, /*top face*/
                6, 1, 2, 2, 7, 6, /*right face*/
                0, 1, 6, 6, 5, 0, /*bottom face*/
                5, 6, 7, 7, 8, 5 /*front face*/ };
            params.indices = cubeIndices;
            params.indicesCount = sizeof(cubeIndices)/sizeof(cubeIndices[0]);
            params.obj = triangle; // We reuse the same vertex buffer that triangle uses
            cube->setup(params);
        }
        std::shared_ptr<Object> rectangleObj(new Object());
        rectangleObj->translate.z = 50.0f;
        scene.add(Item(rectangle, rectangleObj));
        scene.add(Item(triangle, mTriangleObj));
        mCubeObj->scale.x = 0.5f; mCubeObj->scale.y = 0.5f; mCubeObj->scale.z = 0.5f;
        scene.add(Item(cube, mCubeObj));
        std::srand(static_cast<unsigned int>(time(NULL)));
        for(int i = 0; i < 10; ++i )
        {
            std::shared_ptr<Object> obj(new Object());
            for(int j = 0; j < obj->translate.length(); ++j)
                obj->translate[j] = (static_cast<float>(std::rand() % 200) - 100.0f)/100.0f;
            int pi1000 = static_cast<int>(glm::pi<float>() * 1000.0f);
            for(int j = 0; j < obj->rotate.length(); ++j)
                obj->rotate[j] = static_cast<float>(std::rand() % pi1000)/static_cast<float>(pi1000);
            for(int j = 0; j < obj->scale.length(); ++j)
                obj->scale[j] = 0.05f + 0.1f * static_cast<float>(std::rand() % 50)/100.0f;
            scene.add(Item(cube, obj));
        }
    }

    void
    tearDown()
    {
    }

    std::shared_ptr<Object> mTriangleObj;
    std::shared_ptr<Object> mCubeObj;

    static const char* mCrateTexturePath;
    static const char* mSmileTexturePath;
};

const char* SceneData::mCrateTexturePath = "../../../dunca/res/container.jpg";
const char* SceneData::mSmileTexturePath = "../../../dunca/res/awesome_face.png";

class App: public Window::IKeyCallback, public Renderer::IRenderEvents
{
public:

    int
    exec()
    {
        try
        {
            int screenWidth = 800;
            int screenHeight = 600;
            std::shared_ptr<Window> window = Window::init(screenWidth, screenHeight);
            window->addKeyHandler(this);

            std::shared_ptr<Scene> scene(new Scene);
            scene->init();

            // Setup scene
            mSceneData.setup(*scene);

            mCamera.reset(new Camera(0, 0, screenWidth, screenHeight,
                45.0f, static_cast<float>(screenWidth)/static_cast<float>(screenHeight), 0.1f, 100.0f));

            Renderer render;
            render.init(window, scene, mCamera);
            render.addRenderEventListerner(this);

            render.runLoop();

            // Cleanup scene
            mSceneData.tearDown();

            glfwTerminate();
        }
        catch(std::exception e)
        {
            std::cout << e.what() << '\n';
            return -1;
        }
        return 0;
    }

    void
    keyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if(action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            static const float degrees = glm::pi<float>()/18.0f;
            bool done = true;
            switch(key)
            {
            case GLFW_KEY_UP:
            case GLFW_KEY_DOWN:
                {
                    mSceneData.mTriangleObj->rotate.x += degrees * (key == GLFW_KEY_UP ? -1.0f : 1.0f);
                } break;
            case GLFW_KEY_LEFT:
            case GLFW_KEY_RIGHT:
                {
                    mSceneData.mTriangleObj->rotate.y += degrees * (key == GLFW_KEY_LEFT ? -1.0f : 1.0f);
                } break;
            case GLFW_KEY_W:
            case GLFW_KEY_S:
                {
                    mSceneData.mTriangleObj->translate.z += degrees * (key == GLFW_KEY_W ? 1.0f : -1.0f);
                } break;
            case GLFW_KEY_A:
            case GLFW_KEY_D:
                {
                    mSceneData.mTriangleObj->translate.x += degrees * (key == GLFW_KEY_A ? 1.0f : -1.0f);
                } break;
            case GLFW_KEY_KP_ADD:
            case GLFW_KEY_KP_SUBTRACT:
                {
                    for(int j = 0; j < mSceneData.mCubeObj->scale.length(); ++j)
                        mSceneData.mCubeObj->scale[j] += (key == GLFW_KEY_KP_ADD) ? 0.1f : -0.1f;
                } break;
            default: break;
            }
        }

        // When a user presses the escape key, we set the WindowShouldClose property to true, 
        // closing the application
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            mWindow->term();
        }
    }

    void beginFrame()
    {
        assert(mCamera != nullptr);
        mSceneData.mCubeObj->rotate.x = glm::radians((GLfloat)glfwGetTime() * 25.0f);
        mSceneData.mCubeObj->rotate.y = glm::radians((GLfloat)glfwGetTime() * 50.0f);
        static const GLfloat radius = 3.0f;
        mCamera->setPos(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius));
    }

private:
    std::shared_ptr<Window> mWindow;
    std::shared_ptr<Camera> mCamera;
    SceneData mSceneData;
};

int main()
{
    App a;
    return a.exec();
}
