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

// tutorial specific
#include "objects.hpp"
#include "FPSCamera.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

FPSCamera camera(glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = false;
float deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
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

        window = glfwCreateWindow(800, 600, "Coordinate Systems", NULL, NULL);
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
        glfwSetCursorPosCallback(window, &mouse_callback);
        glfwSetScrollCallback(window, &scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        LGL::GLProgram program;
        program.LoadVertexShader("D:\\Projects\\workgroupOpenGL\\LearnOpenGL\\CoordinateSystem\\vertextTransf.vs");
        program.LoadFragmentShader("D:\\Projects\\workgroupOpenGL\\LearnOpenGL\\CoordinateSystem\\fragment.frag");

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

        LGL::VertexArrayObject vao;
        vao.Begin();
        vao.BindBuffer(0, objects::vertices_square, 4, &objects::VertexEntry::m_Position);
        vao.BindBuffer(1, objects::vertices_square, 4, &objects::VertexEntry::m_Color);
        vao.BindBuffer(2, objects::vertices_square, 4, &objects::VertexEntry::m_TextCoord);
        vao.BindIndex(objects::indices_square, 2);
        vao.Done();

        LGL::VertexArrayObject vao_cube;
        vao_cube.Begin();
        vao_cube.BindBuffer(0, objects::vertices_cube, 36, &objects::VertexEntry::m_Position);
        vao_cube.BindBuffer(1, objects::vertices_cube, 36, &objects::VertexEntry::m_Color);
        vao_cube.BindBuffer(2, objects::vertices_cube, 36, &objects::VertexEntry::m_TextCoord);
        //vao.BindIndex(objects::indices_square, 2);
        vao_cube.Done();

        LGL::Uniform(program, "ourTexture1").Set(0);
        LGL::Uniform(program, "ourTexture2").Set(1);
        
        glm::mat4 modelMat;
        modelMat = glm::rotate( modelMat, glm::radians( -55.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
        glm::mat4 viewMat;
        viewMat = camera.getViewMatrix();//glm::translate( viewMat, glm::vec3( 0.0f, 0.0f, -3.0f ) );
        glm::mat4 projMat;
        projMat = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );
        program.Use();
        GLuint modelMatLoc = glGetUniformLocation( program.GetProgram(), "model");
        GLuint viewMatLoc = glGetUniformLocation( program.GetProgram(), "view");
        GLuint projMatLoc = glGetUniformLocation( program.GetProgram(), "projection");
        glEnable(GL_DEPTH_TEST);

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
            GLfloat timeValue = static_cast<GLfloat>( glfwGetTime() );
            deltaTime = timeValue - lastFrame;
            lastFrame = timeValue;
            GLfloat positionOffsetValue = (sin(timeValue) / 2);
            glUniformMatrix4fv( viewMatLoc, 1, GL_FALSE, glm::value_ptr( viewMat ) );
            glUniformMatrix4fv( projMatLoc, 1, GL_FALSE, glm::value_ptr( projMat ) );
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            texture1.Use(GL_TEXTURE0);
            texture2.Use(GL_TEXTURE1);
            
            viewMat = camera.getViewMatrix();
            glUniformMatrix4fv( viewMatLoc, 1, GL_FALSE, glm::value_ptr( viewMat ) );
            for( int i = 0; i < 10; ++i )
            {
                modelMat = glm::mat4();
                modelMat = glm::translate( modelMat, cubePositions[i] );
                modelMat = glm::rotate( modelMat, glm::radians( 0.35f * 10 * i * timeValue * ( i%3==0 ? 1.0f : 0.0f ) ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
                glUniformMatrix4fv( modelMatLoc, 1, GL_FALSE, glm::value_ptr( modelMat ) );
                vao_cube.DrawTriangles();
            }

            //vao_cube.DrawTriangles();
            
            glfwSwapBuffers(window);
        }

        glfwTerminate();
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;	
    }
    if(keys[GLFW_KEY_W])
        camera.updateMovement(ICamera::FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.updateMovement(ICamera::BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.updateMovement(ICamera::LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.updateMovement(ICamera::RIGHT, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xpos;
    lastY = ypos;

    camera.updateRotation(xoffset, yoffset);
}	


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
}