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

struct VaoObject
{
    // We want as much data in here as we could in order to
    // minimizes switching of VBOs
    GLuint mVbo;
    GLuint mVerticesCount;

public:
    GLuint mProgram; // Transformation and material
    GLuint mVao;

    void setup(GLuint vertexAttribPos, const GLfloat* triangleV, GLuint verticesCount)
    {
        // Setup vertex buffer to keep the vertices
        glGenBuffers( 1, &mVbo );
        // Bind the newly created buffer to the array buffer target
        glBindBuffer( GL_ARRAY_BUFFER,  mVbo );
        // Upload buffer to GPU in its provided location
        glBufferData( GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), triangleV, GL_STATIC_DRAW );
        mVerticesCount = verticesCount;

        // Generate VAO to represent object data on GPU
        glGenVertexArrays(1, &mVao);
        // Start binding appropriate data to this VAO
        glBindVertexArray(mVao);
        {
            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            // Setup properties vertex data attribute
            glVertexAttribPointer(vertexAttribPos, 3, GL_FLOAT, GL_FALSE,
                3*sizeof(GLfloat), static_cast<GLvoid*>(0));
            glEnableVertexAttribArray(vertexAttribPos);
        }
        // End of binding VAO setup
        glBindVertexArray(0);
    }

    void draw()
    {
        glUseProgram(mProgram);
        glBindVertexArray(mVao);
        // Draw triangles
        glDrawArrays(GL_TRIANGLES, 0, mVerticesCount);
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

    static VaoObject triangle;
    static VaoObject rectangle;

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

        triangle.mProgram = state.redProgramId;
        // Counterclockwise to look at the front-face ( screen space )
        const GLfloat triangleV[] = { -0.8f, -0.8f, 0.0f,
            0.8f, -0.8f, 0.0f,
            0.0f,  0.8f, 0.0f };
        const GLuint triangleVVertCount = sizeof(triangleV)/sizeof(triangleV[0]);
        triangle.setup(state.vertexAttribPos, triangleV, triangleVVertCount);

        rectangle.mProgram = state.greenProgramId;
        const GLfloat rectangleV[] = { -0.8f, -0.8f, 0.0f,
            0.8f, -0.8f, 0.0f,
            0.8f,  0.8f, 0.0f,
            0.8f,  0.8f, 0.0f,
           -0.8f,  0.8f, 0.0f,
           -0.8f, -0.8f, 0.0f};
        const GLuint rectangleVVertCount = sizeof(rectangleV)/sizeof(rectangleV[0]);
        rectangle.setup(state.vertexAttribPos, rectangleV, rectangleVVertCount);

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

VaoObject SceneData::triangle;
VaoObject SceneData::rectangle;

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
