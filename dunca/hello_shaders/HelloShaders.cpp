#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

#include "shader.h"


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
    StateData()
        : vertexAttribPos(0) // Bind vertex data to first attribute "layout (location = 0)"
        , colorAttribPos(1) // Bind color data to the second attribute "layout (location = 1)"
    {

    }

    const GLuint vertexAttribPos;
    const GLuint colorAttribPos;
};

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

    Shader mProgram; // Transformation and material
    GLuint mVao;

    virtual bool configUniforms()
    {
        return true; // Do nothing, just a way for subclass to retrive uniforms
    }

    virtual bool updateUniforms()
    {
        return true; // Do nothing, just a way for subclass to update uniforms
    }

    void setup(GLuint vertexAttribPos, GLuint vertexAttribColor,
        const GLfloat* vertices, GLuint verticesCount,
        const GLuint* indices = NULL, GLuint indicesCount = 0,
        Object* obj = NULL )
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
            glVertexAttribPointer(vertexAttribPos, 3, GL_FLOAT, GL_FALSE,
                ((vertexAttribColor != -1) ? 6 : 3)*sizeof(GLfloat), static_cast<GLvoid*>(0));
            if(vertexAttribColor != -1)
            {
                glVertexAttribPointer(vertexAttribColor, 3, GL_FLOAT, GL_FALSE,
                    6*sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
                glEnableVertexAttribArray(vertexAttribColor);
            }
            glEnableVertexAttribArray(vertexAttribPos);
        }
        // End of binding VAO setup
        glBindVertexArray(0);

        return configUniforms();
    }

    void draw()
    {
        mProgram.Use();
        // Update uniforms after program is set otherwise glUniform* calls won't work
        updateUniforms();

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
};

class TriangleObject: public Object
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

    static TriangleObject triangle;
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

        // Set program before setup where uniforms location is retrieved
        bool ok = triangle.mProgram.load(scaleVertexShaderSz, redFragmentShaderSz);
        // Counterclockwise to look at the front-face ( screen space )
        const GLuint triangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/,
            3/*top-center*/ };
        ok = ok && triangle.setup(state.vertexAttribPos, state.colorAttribPos, // This should correspond to vertex shader attribute layout
            allVertices, sizeof(allVertices)/sizeof(allVertices[0]),
            triangleIndices, sizeof(triangleIndices)/sizeof(triangleIndices[0]));
        if(ok)
        {
            // Set program before setup where uniforms location is retrieved
            rectangle.mProgram.load(vertexShaderSz, greenFragmentShaderSz);
            const GLuint rectangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/,
                2/*top-right*/, 2/*top-right*/, 4/*top-left*/, 0/*bottom-left*/ };
            ok = rectangle.setup(state.vertexAttribPos, state.colorAttribPos, NULL, 0,
                rectangleIndices, sizeof(rectangleIndices)/sizeof(rectangleIndices[0]),
                &triangle ); // We reuse the same vertex buffer that triangle uses
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

TriangleObject SceneData::triangle;
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