#define GLEW_STATIC

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "GLProgram.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
}


typedef GLfloat Vec3f[3];

struct VertexEntry
{
    Vec3f m_Position;
    Vec3f m_Color;
};

struct VertexArrayObject
{
    VertexArrayObject()
    : m_Vao()
    , m_Ebo()
    , m_BufferObjectAddress()
    , m_Count()
    , m_DrawCount()
    , m_Vbo()
    {
    }

    ~VertexArrayObject()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glDeleteBuffers(1, &m_Vao);
        glDeleteBuffers(1, &m_Vbo);
        glDeleteBuffers(1, &m_Ebo);
    }

    void Begin()
    {
        glGenBuffers(1, &m_Vbo);
        glGenBuffers(1, &m_Ebo);

        glGenVertexArrays(1, &m_Vao);
        glBindVertexArray(m_Vao);        
    }
    
    void Done()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    template <typename VERTEX_ENTRY, typename ELEMENT>
    void BindBuffer(unsigned int location, const VERTEX_ENTRY * buffer, unsigned int count, ELEMENT VERTEX_ENTRY::*offset)
    {
        assert(m_Vbo != 0);

        assert(m_BufferObjectAddress == NULL || m_BufferObjectAddress == buffer);
        assert(m_Count == 0 || m_Count == count);

        if (m_BufferObjectAddress == NULL)
        {   
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_ENTRY) * count, buffer, GL_STATIC_DRAW);
        }

        m_BufferObjectAddress = buffer;
        m_Count = count;
        
        // !TODO
        // map types and sizes from ELEMENT in a generic way

        const GLuint elementSize = sizeof(ELEMENT) / sizeof( GLfloat );
        const GLuint strideSize = sizeof(VERTEX_ENTRY);
        const GLvoid * relativeOffset = static_cast<const GLvoid*>(&(static_cast<VERTEX_ENTRY*>(0)->*offset));
        glVertexAttribPointer(location, elementSize, GL_FLOAT, GL_FALSE, strideSize, relativeOffset );
        glEnableVertexAttribArray(location);
    }


    void BindIndex(GLuint indices[][3], unsigned int count)
    {
        assert(m_Ebo != 0);
        // !TODO
        // infert the primitive type from the Index call
        m_DrawCount = count;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * count, indices, GL_STATIC_DRAW);
    }

    void Draw()
    {
        typedef GLuint IndexType[3];

        assert(m_Vao != NULL);
        assert(m_Count != 0);
        assert(m_DrawCount != 0);

        glBindVertexArray(m_Vao);
        glDrawElements(GL_TRIANGLES, m_DrawCount * sizeof(IndexType), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    GLuint m_Vao;
    GLuint m_Ebo;

    const void * m_BufferObjectAddress;
    unsigned int m_Count;

    unsigned int m_DrawCount;
    GLuint m_Vbo;
    
};

struct Uniform
{
    Uniform(LGL::GLProgram & program, const char * name)
    : m_Location()
    {
        program.Use();
        m_Location = glGetUniformLocation(program.GetProgram(), name);
        if (m_Location == -1)
            throw LGL::GLException("Failed to get uniform location");
    }

    ~Uniform()
    {
    }

    void Set(GLfloat x)
    {
        glUniform1f(m_Location, x);
    }

private:
    GLuint m_Location;
};

int main()
{
    try {
        GLFWwindow* window;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window = glfwCreateWindow(800, 600, "Shaders", NULL, NULL);
        glfwMakeContextCurrent(window);
        if (window == NULL)
        {
            printf("Failed to create GLFW window\n");
            glfwTerminate();
            return -1;
        }

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            printf("Failed to initialize GLEW\n");
            return -1;
        }

        glViewport(0, 0, 800, 600);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSetKeyCallback(window, &KeyCallback);

        LGL::GLProgram program;
        program.LoadVertexShader("LearnOpenGL/Shaders/Shaders.vs");
        program.LoadFragmentShader("LearnOpenGL/Shaders/Shaders.frag");

        VertexEntry vertices[3] = {
            {
                { 0.0f, 0.5f, 0.0f },      // Bottom
                { 1.0f, 0.0f, 0.0f },
            },
            {
                { -0.5f, -0.5f, 0.0f },     // Top Left
                { 0.0f, 1.0f, 0.0f },
            },
            {
                { 0.5f, -0.5f, 0.0f },      // Top Right
                { 0.0f, 0.0f, 1.0f },
            }
        };
        
        GLuint indices[][3] = {
            { 0, 1, 2 }
        };

        VertexArrayObject vao;
        vao.Begin();
        vao.BindBuffer(0, vertices, 3, &VertexEntry::m_Position);
        vao.BindBuffer(1, vertices, 3, &VertexEntry::m_Color);
        vao.BindIndex(indices, 1);
        vao.Done();

        Uniform u(program, "positionOffset");
                
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            program.Use();

            GLfloat timeValue = static_cast<GLfloat>( glfwGetTime() );
            GLfloat positionOffsetValue = (sin(timeValue) / 2);

            u.Set( positionOffsetValue );

            vao.Draw();
            
            glfwSwapBuffers(window);
        }

        glfwTerminate();

    }
    catch (const LGL::GLException & exception)
    {
        std::cerr << exception.what() << "\n";
    }

    return 0;
}
