#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
    StateData()
        : vertexAttribPos(0) // Bind vertex data to first attribute "layout (location = 0)"
    {

    }

    GLuint vertexShader;
    GLuint redFragmentShader;
    GLuint greenFragmentShader;
    GLuint redProgramId;
    GLuint greenProgramId;
    const GLuint vertexAttribPos;
};

struct Object
{
    // We want as much data in here as we could in order to
    // minimizes switching of VBOs
    GLuint mVbo;
    GLuint mVerticesCount;
    GLuint mEbo;
    GLuint mIndicesCount;
    bool mHasEbo;

public:
    GLuint mProgram; // Transformation and material
    GLuint mVao;

    bool setup(GLuint vertexAttribPos,
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
                3*sizeof(GLfloat), static_cast<GLvoid*>(0));
            glEnableVertexAttribArray(vertexAttribPos);
        }
        // End of binding VAO setup
        glBindVertexArray(0);
        return true;
    }

    void draw()
    {
        glUseProgram(mProgram);
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

bool
compileShader(const char* source, GLuint& id, bool vertexShader)
{
    // Create a new shader
    id = glCreateShader( vertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER );
    // Provide shader source
    glShaderSource(id, 1, &source, NULL);
    // Compile shader
    glCompileShader(id);

    GLint compiled = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        GLchar errorMessage[512];
        glGetShaderInfoLog(vertexShader, sizeof(errorMessage), NULL, errorMessage);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errorMessage << std::endl;
    }
    return (compiled == GL_TRUE);
}

bool
createProgram(GLuint& id, GLuint* firstShader, GLuint* secondShader)
{
    bool ok = true;
    id = glCreateProgram();
    if(firstShader != NULL)
    {
        glAttachShader(id, *firstShader);
    }
    if(secondShader != NULL)
    {
        glAttachShader(id, *secondShader);
    }
    glLinkProgram(id);
    GLint linked = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLchar errorMessage[512];
        glGetProgramInfoLog(id, sizeof(errorMessage), NULL, errorMessage);
    }
    return ok;
}

struct SceneData
{
    // Inline shaders source code
    static const char* vertexShaderSz;
    static const char* redFragmentShaderSz;
    static const char* greenFragmentShaderSz;

    static Object triangle;
    static Object rectangle;

    static bool
    setup(StateData& state )
    {
        // Compile both shaders
        bool ok = compileShader(vertexShaderSz, state.vertexShader, true);
        ok = ok && compileShader(redFragmentShaderSz, state.redFragmentShader, false);
        ok = ok && createProgram(state.redProgramId, &state.vertexShader,
            &state.redFragmentShader);
        ok = ok && compileShader(greenFragmentShaderSz, state.greenFragmentShader, false);
        ok = ok && createProgram(state.greenProgramId, &state.vertexShader,
            &state.greenFragmentShader);
        // Cleanup shaders
        glDeleteShader(state.vertexShader);
        glDeleteShader(state.redFragmentShader);
        glDeleteShader(state.greenFragmentShader);

        if(ok)
        {
            const GLfloat allVertices[] = {
               -0.8f, -0.8f, 0.0f, // bottom-left
                0.8f, -0.8f, 0.0f, // bottom-right
                0.8f,  0.8f, 0.0f, // top-right
                0.0f,  0.8f, 0.0f, // top-center
               -0.8f,  0.8f, 0.0f }; // top-left

            triangle.mProgram = state.redProgramId;
            // Counterclockwise to look at the front-face ( screen space )
            const GLuint triangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/,
                3/*top-center*/ };
            ok = triangle.setup(state.vertexAttribPos, // This should correspond to vertex shader attribute layout
                allVertices, sizeof(allVertices)/sizeof(allVertices[0]),
                triangleIndices, sizeof(triangleIndices)/sizeof(triangleIndices[0]));
            if(ok)
            {
                rectangle.mProgram = state.greenProgramId;
                const GLuint rectangleIndices[] = { 0/*bottom-left*/, 1/*bottom-right*/,
                    2/*top-right*/, 2/*top-right*/, 4/*top-left*/, 0/*bottom-left*/ };
                ok = rectangle.setup(state.vertexAttribPos, NULL, 0,
                    rectangleIndices, sizeof(rectangleIndices)/sizeof(rectangleIndices[0]),
                    &triangle ); // We reuse the same vertex buffer that triangle uses
            }
        }
        return ok;
    }

    static void
    tearDown(StateData& state)
    {
        glDeleteProgram(state.redProgramId);
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
const char* SceneData::vertexShaderSz ="\n\
                                       #version 330 core\n\
                                       layout (location = 0) in vec3 position;\n\
                                       void main()\n\
                                       {\n\
                                       gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n\
                                       }";

const char* SceneData::redFragmentShaderSz = "\n\
                                          #version 330 core\n\
                                          out vec4 color;\n\
                                          void main()\n\
                                          {\n\
                                          color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n\
                                          }";

const char* SceneData::greenFragmentShaderSz = "\n\
                                             #version 330 core\n\
                                             out vec4 color;\n\
                                             void main()\n\
                                             {\n\
                                             color = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n\
                                             }";

int main()
{
    GLFWwindow* window;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 600, "Hello Triangle (dunca)", NULL, NULL);
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
    SceneData::setup(state);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Draw scene
        SceneData::drawFrame(state);

        glfwSwapBuffers(window);
    }

    // Cleanup scene
    SceneData::tearDown(state);

    glfwTerminate();

    return 0;
}
