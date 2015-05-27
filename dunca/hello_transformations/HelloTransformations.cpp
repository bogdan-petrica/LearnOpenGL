#define GLEW_STATIC

#include "tools.h"

#include "object.h"
#include "shader.h"
#include "material.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <stdio.h>
#include <iostream>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

const GLfloat bkColor[3] = { 0.2f, 0.3f, 0.3f };

struct StateData
{
};

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

const char* ColorMaterial::vertexShaderSz ="../../../dunca/hello_shaders/green.vs";
const char* ColorMaterial::greenFragmentShaderSz = "../../../dunca/hello_shaders/green.frag";

class TextureMaterial: public Material
{
public:

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
        mCrateTextLoc = glGetUniformLocation(mProgram.GetProgram(), "crateTextureUnit");
        mSmileTextLoc = glGetUniformLocation(mProgram.GetProgram(), "smileTextureUnit");
        return mUniformLocation != -1;
    }

    bool 
    updateUniforms()
    {
        glUniform1f(mUniformLocation, static_cast<GLfloat>( abs(sin(glfwGetTime())) ));
        glUniform1i(mCrateTextLoc, 0);
        glUniform1i(mSmileTextLoc, 1);
        return true; // Do nothing, just a way for subclass to update uniforms
    }

    GLuint vertexAttribPos() const { return 0; }; // Bind vertex data to first attribute "layout (location = 0)"
    GLuint colorAttribPos() const { return 1; }; // Bind color data to the second attribute "layout (location = 1)"
    GLuint textCoordAttribPos() const { return 2; }; // Bind texture data to the third attribute "layout (location = 2)"

private:
    GLuint mUniformLocation;
    GLuint mCrateTextLoc;
    GLuint mSmileTextLoc;

    // Inline shaders source code
    static const char* scaleTextureVertexShaderSz;
    static const char* textureFragmentShaderSz;
};

const char* TextureMaterial::scaleTextureVertexShaderSz ="../../../dunca/hello_textures/texture.vs";
const char* TextureMaterial::textureFragmentShaderSz = "../../../dunca/hello_textures/texture.frag";

struct SceneData
{
    static Object triangle;
    static Object rectangle;

    static bool
    setup(StateData& state )
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const static GLfloat KRadius = 0.9f;
        const GLfloat allVertices[] = {
               -KRadius, -KRadius, 0.0f,    0.0f, 0.0f,    1.0f, 0.0f, 0.0f, // bottom-left
                KRadius, -KRadius, 0.0f,    1.0f, 0.0f,    0.0f, 1.0f, 0.0f, // bottom-right
                KRadius,  KRadius, 0.0f,    1.0f, 1.0f,    0.0f, 1.0f, 1.0f, // top-right
                0.0f,  KRadius, 0.0f,       0.5f, 1.0f,    0.5f, 1.0f, 0.5f, // top-center
               -KRadius,  KRadius, 0.0f,    0.0f, 1.0f,    1.0f, 1.0f, 0.0f }; // top-left
        
        std::shared_ptr<Texture> triangleTextures = std::make_shared<Texture>();
        bool ok = triangleTextures->loadFromFile(mCrateTexturePath);
        ok = ok && triangleTextures->loadFromFile(mSmileTexturePath, true);
        std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();
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
            params.vertices = allVertices;
            params.verticesCount = sizeof(allVertices)/sizeof(allVertices[0]);
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

    static void
    tearDown(StateData& state)
    {
    }

    static void
    drawFrame(StateData& state)
    {
        // Draw background to cover the previous frame
        glClearColor( bkColor[0], bkColor[1], bkColor[2], 1.0f );
        glClear(GL_COLOR_BUFFER_BIT);

        // Every 100 frames switch to drawing rectangle and vice versa
        rectangle.draw();
        // Draw the freaking triangle
        triangle.draw();
    }

private:
    static const char* mCrateTexturePath;
    static const char* mSmileTexturePath;
};

const char* SceneData::mCrateTexturePath = "../../../dunca/res/container.jpg";
const char* SceneData::mSmileTexturePath = "../../../dunca/res/awesome_face.png";

Object SceneData::triangle;
Object SceneData::rectangle;

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
    StateData state;
    bool ok = SceneData::setup(state);

    while(ok && !glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Draw scene
        SceneData::drawFrame(state);

        glfwSwapBuffers(window);
    }

    // Cleanup scene
    SceneData::tearDown(state);

    glfwTerminate();

    return ok ? 0 : -1;
}
