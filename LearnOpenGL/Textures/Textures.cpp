#define GLEW_STATIC

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// Common
#include "GLProgram.h"
#include "VertexArrayObject.h"
#include "Uniform.h"

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

int main()
{
    try {
        GLFWwindow* window;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window = glfwCreateWindow(800, 600, "Textures", NULL, NULL);
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
        program.LoadVertexShader("LearnOpenGL/Textures/Textures.vs");
        program.LoadFragmentShader("LearnOpenGL/Textures/Textures.frag");

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

        LGL::VertexArrayObject vao;
        vao.Begin();
        vao.BindBuffer(0, vertices, 3, &VertexEntry::m_Position);
        vao.BindBuffer(1, vertices, 3, &VertexEntry::m_Color);
        vao.BindIndex(indices, 1);
        vao.Done();

        LGL::Uniform u(program, "positionOffset");
                
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
