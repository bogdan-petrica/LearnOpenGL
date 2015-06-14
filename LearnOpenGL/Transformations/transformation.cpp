#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Common
#include "GLProgram.h"
#include "VertexArrayObject.h"
#include "Uniform.h"

// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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

        void Load( const char * path, GLuint texturedID = 0 )
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
            glBindTexture(GL_TEXTURE_2D, texturedID );
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
            switch( unitIdx )
            {
                case GL_TEXTURE0:
                {
                    glActiveTexture(GL_TEXTURE0 + unitIdx);
                    glBindTexture(GL_TEXTURE_2D, m_Texture);
                    break;
                }
                case GL_TEXTURE1:
                {
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, m_Texture);
                    break;
                }
                default:
                    break;
            }
        }

    
    private:
        GLuint m_Texture;
    };
}



int main()
{ 
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

        glfwSetKeyCallback(window, &key_callback);

        LGL::GLProgram program;
        program.LoadVertexShader("D:\\Projects\\workgroupOpenGL\\LearnOpenGL\\Transformations\\vertextTransf.vs");
        program.LoadFragmentShader("D:\\Projects\\workgroupOpenGL\\LearnOpenGL\\Transformations\\fragment.frag");

        LGL::Texture texture1;
        texture1.Load("D:\\Projects\\workgroupOpenGL\\LearnOpenGL\\Texture\\container.jpg");
        texture1.SetClamp(LGL::TextureAxisS);
        texture1.SetClamp(LGL::TextureAxisZ);
        texture1.SetMagnifyingFilter(false);

        LGL::Texture texture2;
        texture2.Load("D:\\Projects\\workgroupOpenGL\\LearnOpenGL\\Texture\\awesomeface.png", 1 );
        texture2.SetRepeat(LGL::TextureAxisS);
        texture2.SetRepeat(LGL::TextureAxisZ);
        texture2.SetMagnifyingFilter(false);

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
                { 1.0f , 1.0f }
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

        LGL::Uniform(program, "ourTexture1").Set(0);
        LGL::Uniform(program, "ourTexture2").Set(1);

        glm::mat4 transfMat;
        transfMat = glm::rotate( transfMat, glm::radians( 90.0f ), glm::vec3( 0.0, 0.0, 1.0 ) );
        transfMat = glm::scale( transfMat, glm::vec3( 0.5, 0.5, 0.5 ) );
        
                
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            GLfloat timeValue = static_cast<GLfloat>( glfwGetTime() );
            GLfloat positionOffsetValue = (sin(timeValue) / 2);

            glm::mat4 translateOp;
            translateOp = glm::rotate( translateOp, glm::radians( timeValue * 50.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
            translateOp = glm::translate(translateOp, glm::vec3( 0.5f, -0.5f, 0.0 ) );
            //translateOp = glm::rotate( translateOp, glm::radians( timeValue * 50.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) ); // original tut order

            glm::mat4 secondContainerOp;
            secondContainerOp = glm::translate( secondContainerOp, glm::vec3( -0.5f, 0.5f, 0.0 ) );
            secondContainerOp = glm::scale( secondContainerOp, glm::vec3( std::abs( positionOffsetValue ), std::abs( positionOffsetValue ), std::abs( positionOffsetValue ) ) );

            // read from global variable and set the uniform for mixing the two textures
            GLuint transformLoc = glGetUniformLocation( program.GetProgram(), "transform");
            glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr( translateOp ) ); // was transfMat for tutorial

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            texture1.Use(GL_TEXTURE0);
            texture2.Use(GL_TEXTURE1);

            program.Use();

            
            //u.Set( positionOffsetValue );

            vao.Draw();

            // for 2nd container modify transf and update the  uniform
            glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr( secondContainerOp ) ); // was transfMat for tutorial

            vao.Draw();
            
            glfwSwapBuffers(window);
        }

        glfwTerminate();
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
