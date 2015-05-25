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

#include "SOIL.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
}


struct VertexEntry
{
    LGL::Vec3f m_Position;
    LGL::Vec3f m_Color;
    LGL::Vec2f m_TextCoord;
};

namespace LGL {

    struct Texture
    {
        Texture()
        : m_Texture(0)
        {
        }

        ~Texture()
        {
            if (m_Texture != 0)
                glDeleteTextures(1, &m_Texture);
        }

        void Load( const char * path )
        {
            //glGenTextures(1, &m_Texture);

            int width, height;
            unsigned char * image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
            if (image == 0)
                throw GLException("Failed to load SOIL image");

            glGenTextures(1, &m_Texture);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            SOIL_free_image_data(image);
        }

        void Use()
        {
            assert(m_Texture != NULL);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
        }

    private:
        GLuint m_Texture;
    };
}

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

        LGL::Texture texture;
        texture.Load("LearnOpenGL/Textures/container.jpg");

        VertexEntry vertices[4] = {
            {
                // bottom left
                { -0.5f, -0.5f, 0.0f },             // coordinate
                { 1.0f, 0.0f, 0.0f },               // color
                { 0.0f, 0.0f }                      // texture coordinate
            },
            {
                // bottom right
                { 0.5f, -0.5f, 0.0f },      
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f }
            },
            {
                // top right
                { 0.5f, 0.5f, 0.0f },      
                { 1.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f }
            },

            {
                // top left
                { -0.5f, 0.5f, 0.0f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f }
            }
        };
        
        GLuint indices[][3] = {
            { 0, 1, 3 },
            { 2, 3, 1 }
        };

        LGL::VertexArrayObject vao;
        vao.Begin();
        vao.BindBuffer(0, vertices, 4, &VertexEntry::m_Position);
        vao.BindBuffer(1, vertices, 4, &VertexEntry::m_Color);
        vao.BindBuffer(2, vertices, 4, &VertexEntry::m_TextCoord);
        vao.BindIndex(indices, 2);
        vao.Done();

        LGL::Uniform u(program, "positionOffset");
                
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            texture.Use();

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
