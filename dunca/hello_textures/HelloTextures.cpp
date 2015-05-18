#define GLEW_STATIC

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
    bool
    configUniforms()
    {
        mUniformLocation = glGetUniformLocation(mProgram.Program, "arg");
        return mUniformLocation != -1;
    }

    bool 
    updateUniforms()
    {
        glUniform1f(mUniformLocation, static_cast<GLfloat>( abs(sin(glfwGetTime())) ));
        return true; // Do nothing, just a way for subclass to update uniforms
    }
    
    GLuint vertexAttribPos() const { return 0; }; // Bind vertex data to first attribute "layout (location = 0)"
    GLuint colorAttribPos() const { return 1; }; // Bind color data to the second attribute "layout (location = 1)"

private:
    GLuint mUniformLocation;
};

class TextureMaterial: public Material
{
public:
    bool
    configUniforms()
    {
        return true;
    }

    bool 
    updateUniforms()
    {
        return true; // Do nothing, just a way for subclass to update uniforms
    }

    GLuint vertexAttribPos() const { return 0; }; // Bind vertex data to first attribute "layout (location = 0)"
    GLuint colorAttribPos() const { return 1; }; // Bind color data to the second attribute "layout (location = 1)"

private:
    GLuint mUniformLocation;
};


struct SceneData
{
    // Inline shaders source code
    static const char* scaleVertexShaderSz;
    static const char* vertexShaderSz;
    static const char* redFragmentShaderSz;
    static const char* greenFragmentShaderSz;

    static Object triangle;
    static Object rectangle;

    static bool
    setup(StateData& state )
    {
        const GLfloat allVertices[] = {
               -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
                0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom-right
                0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 1.0f,// top-right
                0.0f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,// top-center
               -0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f }; // top-left
        
        std::unique_ptr<ColorMaterial> colorMaterial(new ColorMaterial());
        bool ok = colorMaterial->mProgram.load(scaleVertexShaderSz, redFragmentShaderSz);
        if(ok)
        {
            // Set program before setup where uniforms location is retrieved
            triangle.mMaterial = colorMaterial.release();

            std::unique_ptr<TextureMaterial> textureMaterial(new TextureMaterial());
            // Set program before setup where uniforms location is retrieved
            ok = textureMaterial->mProgram.load(vertexShaderSz, greenFragmentShaderSz);
            if(ok) rectangle.mMaterial = textureMaterial.release();
        }

        // Counterclockwise to look at the front-face ( screen space )
        const GLuint triangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/,
            3/*top-center*/ };
        ok = ok && triangle.setup(allVertices, sizeof(allVertices)/sizeof(allVertices[0]),
            triangleIndices, sizeof(triangleIndices)/sizeof(triangleIndices[0]), 3 * sizeof(GLfloat));
        const GLuint rectangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/,
            2/*top-right*/, 2/*top-right*/, 4/*top-left*/, 0/*bottom-left*/ };
        ok = rectangle.setup(NULL, 0,
            rectangleIndices, sizeof(rectangleIndices)/sizeof(rectangleIndices[0]), 3 * sizeof(GLfloat),
            &triangle ); // We reuse the same vertex buffer that triangle uses
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

        static unsigned int frameCount = 0;
        if(frameCount % 10000 > 5000)
        {
            // Draw the freaking triangle
            triangle.draw();
        }
        else
        {
            // Every 100 frames switch to drawing rectangle and vice versa
            rectangle.draw();
        }
        ++frameCount;
    }
};

Object SceneData::triangle;
Object SceneData::rectangle;

// Inline shaders source code
// Inline shaders source code
const char* SceneData::scaleVertexShaderSz ="../../../dunca/hello_shaders/multicolor.vs";

const char* SceneData::vertexShaderSz ="../../../dunca/hello_shaders/green.vs";

const char* SceneData::redFragmentShaderSz = "../../../dunca/hello_shaders/multicolor.frag";

const char* SceneData::greenFragmentShaderSz = "../../../dunca/hello_shaders/green.frag";

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
