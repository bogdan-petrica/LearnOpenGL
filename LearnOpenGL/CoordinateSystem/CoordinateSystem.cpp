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

#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLfloat alpha = 0.5f;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        alpha += 0.05f;
        if (alpha > 1.0f)
            alpha = 1.0f;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        alpha -= 0.05f;
        if (alpha < 0.0f)
            alpha = 0.0f;
    }
        
}


struct VertexEntry
{
    LGL::Vec3f m_Position;
    LGL::Vec3f m_Color;
    LGL::Vec2f m_TextCoord;
};

namespace LGL {
    enum TextureAxis {
        TextureAxisS,
        TextureAxisZ
    };

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

        void SetRepeat(TextureAxis axis, bool mirror = false)
        {
            assert(m_Texture != 0);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
            glTexParameteri(GL_TEXTURE_2D, axis == TextureAxisS ? GL_TEXTURE_WRAP_S : GL_TEXTURE_WRAP_T, mirror ? GL_MIRRORED_REPEAT : GL_REPEAT);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
        }

        void SetClamp(TextureAxis axis)
        {
            assert(m_Texture != 0);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
            glTexParameteri(GL_TEXTURE_2D, axis == TextureAxisS ? GL_TEXTURE_WRAP_S : GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
        }


        void SetMagnifyingFilter(bool linear)
        {
            assert(m_Texture != 0);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
        }


        void Use( unsigned int unitIdx )
        {
            assert(m_Texture != 0);
            assert(unitIdx < 16);
            glActiveTexture(GL_TEXTURE0 + unitIdx);
            glBindTexture(GL_TEXTURE_2D, m_Texture);
        }

    
    private:
        GLuint m_Texture;
    };
}

template <typename T, int N>
unsigned int SizeOfArray( const T (&a)[N] )
{
    return N;
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

        window = glfwCreateWindow(800, 600, "CoordinateSystem", NULL, NULL);
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
        program.LoadVertexShader("LearnOpenGL/CoordinateSystem/CoordinateSystem.vs");
        program.LoadFragmentShader("LearnOpenGL/CoordinateSystem/CoordinateSystem.frag");

        LGL::Texture texture1;
        texture1.Load("LearnOpenGL/CoordinateSystem/container.jpg");
        texture1.SetClamp(LGL::TextureAxisS);
        texture1.SetClamp(LGL::TextureAxisZ);
        texture1.SetMagnifyingFilter(false);

        LGL::Texture texture2;
        texture2.Load("LearnOpenGL/CoordinateSystem/awesomeface.png");
        texture2.SetRepeat(LGL::TextureAxisS);
        texture2.SetRepeat(LGL::TextureAxisZ);
        texture2.SetMagnifyingFilter(true);

        VertexEntry vertices[] = {
            {
                // bottom left
                { -0.5f, -0.5f, 0.5f },             // coordinate
                { 1.0f, 0.0f, 0.0f },               // color
                { 0.0f, 0.0f }                      // texture coordinate
            },
            {
                // bottom right
                { 0.5f, -0.5f, 0.5f },      
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f }
            },
            {
                // top right
                { 0.5f, 0.5f, 0.5f },      
                { 1.0f, 1.0f, 0.0f },
                { 1.0f , 1.0f }
            },

            {
                // top left
                { -0.5f, 0.5f, 0.5f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f }
            },

            // right face coordinates
            {
                // bottom left
                { 0.5f, -0.5f, 0.5f },             // coordinate
                { 1.0f, 0.0f, 0.0f },               // color
                { 0.0f, 0.0f }                      // texture coordinate
            },
            {
                // bottom right
                { 0.5f, -0.5f, -0.5f },      
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f }
            },
            {
                // top right
                { 0.5f, 0.5f, -0.5f },      
                { 1.0f, 1.0f, 0.0f },
                { 1.0f , 1.0f }
            },

            {
                // top left
                { 0.5f, 0.5f, 0.5f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f }
            },

            // left face coordinates
            {
                // bottom left
                { -0.5f, -0.5f, -0.5f },              // coordinate
                { 1.0f, 0.0f, 0.0f },               // color
                { 0.0f, 0.0f }                      // texture coordinate
            },
            {
                // bottom right
                { -0.5f, -0.5f, 0.5f },      
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f }
            },
            {
                // top right
                { -0.5f, 0.5f, 0.5f },      
                { 1.0f, 1.0f, 0.0f },
                { 1.0f , 1.0f }
            },

            {
                // top left
                { -0.5f, 0.5f, -0.5f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f }
            },


            {
                // back face
                { 0.5f, -0.5f, -0.5f },             // coordinate
                { 1.0f, 0.0f, 0.0f },               // color
                { 0.0f, 0.0f }                      // texture coordinate
            },
            {
                // bottom right
                { -0.5f, -0.5f, -0.5f },      
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f }
            },
            {
                // top right
                { -0.5f, 0.5f, -0.5f },      
                { 1.0f, 1.0f, 0.0f },
                { 1.0f , 1.0f }
            },

            {
                // top left
                { 0.5f, 0.5f, -0.5f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f }
            },



            {
                // top face
                { -0.5f, 0.5f, 0.5f },             // coordinate
                { 1.0f, 0.0f, 0.0f },               // color
                { 0.0f, 0.0f }                      // texture coordinate
            },
            {
                // bottom right
                { 0.5f, 0.5f, 0.5f },
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f }
            },
            {
                // top right
                { 0.5f, 0.5f, -0.5f },
                { 1.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f }
            },

            {
                // top left
                { -0.5f, 0.5f, -0.5f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f }
            },

            {
                // bottom face
                { -0.5f, -0.5f, 0.5f },             // coordinate
                { 1.0f, 0.0f, 0.0f },               // color
                { 0.0f, 0.0f }                      // texture coordinate
            },
            {
                // bottom right
                { 0.5f, -0.5f, 0.5f },
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f }
            },
            {
                // top right
                { 0.5f, -0.5f, -0.5f },
                { 1.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f }
            },

            {
                // top left
                { -0.5f, -0.5f, -0.5f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f }
            }
        };
        
        GLuint indices[][3] = {

            // FrontFace
            { 0, 1, 3 },
            { 2, 3, 1 },

            // RightFace
            { 4, 5, 7 },
            { 6, 7, 5 },

            // LeftFace
            { 8, 9, 11 },
            { 10, 11, 9 },

            // BackFace
            { 12, 13, 15 },
            { 14, 15, 13 },

            // TopFace
            { 16, 17, 19 },
            { 18, 19, 17 },

            // TopFace
            { 20, 21, 23 },
            { 22, 23, 21 }
        };

        glEnable(GL_DEPTH_TEST);  

        unsigned int VerticesCount = SizeOfArray( vertices );


        LGL::VertexArrayObject vao;
        vao.Begin();
        vao.BindBuffer(0, vertices, VerticesCount, &VertexEntry::m_Position);
        vao.BindBuffer(1, vertices, VerticesCount, &VertexEntry::m_Color);
        vao.BindBuffer(2, vertices, VerticesCount, &VertexEntry::m_TextCoord);
        vao.BindIndex(indices, 12);
        vao.Done();

        LGL::Uniform(program, "ourTexture1").Set(0);
        LGL::Uniform(program, "ourTexture2").Set(1);
        LGL::Uniform mixValue(program, "mixValue");

        LGL::Uniform model(program, "model");
        LGL::Uniform view(program, "view");
        LGL::Uniform proj(program, "proj");


        glm::mat4 viewMat;
        viewMat = glm::translate( viewMat, glm::vec3( 0.0f, 0.0f, -3.0f ) );
        //viewMat = glm::rotate( viewMat, glm::radians( 45.0f ), glm::vec3( 0.0, 1.0f, 0.0 ));
        
        glm::mat4 projMat;
        projMat = glm::perspective( 45.0f, 800.0f / 600.0f, 0.1f, 100.0f );

        glm::vec3 cubePositions[] = {
          glm::vec3( 0.0f,  0.0f,  0.0f), 
          glm::vec3( 2.0f,  5.0f, -15.0f), 
          glm::vec3(-1.5f, -2.2f, -2.5f),  
          glm::vec3(-3.8f, -2.0f, -12.3f),  
          glm::vec3( 2.4f, -0.4f, -3.5f),  
          glm::vec3(-1.7f,  3.0f, -7.5f),  
          glm::vec3( 1.3f, -2.0f, -2.5f),  
          glm::vec3( 1.5f,  2.0f, -2.5f), 
          glm::vec3( 1.5f,  0.2f, -1.5f), 
          glm::vec3(-1.3f,  1.0f, -1.5f)  
        };
        
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // read from global variable and set the uniform for mixing the two textures
            mixValue.Set(alpha);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            texture1.Use(0);
            texture2.Use(1);

            program.Use();

            view.Set( glm::value_ptr( viewMat ) );
            proj.Set( glm::value_ptr( projMat) );

            float roateAngle = (float)glfwGetTime() * 20.00f;

            for( unsigned int  i = 0; i < SizeOfArray( cubePositions); ++i ) {
                glm::mat4 modelMat = glm::mat4();
                                
                modelMat = glm::translate( modelMat, cubePositions[ i ] );

                if ( i % 3 == 0 ) {
                    modelMat = glm::rotate( modelMat,  glm::radians( roateAngle ), glm::vec3( 0.0f, 1.0f, 1.0f ) );
                } else {
                    modelMat = glm::rotate( modelMat,  glm::radians( i * 20.00f ), glm::vec3( 1.0f, 0.3f, 0.5f ) );
                }
                
                model.Set( glm::value_ptr( modelMat ) );
                vao.Draw();
            }

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
