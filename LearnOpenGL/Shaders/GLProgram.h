#ifndef LGL_GLPROGRAM_H
#define LGL_GLPROGRAM_H

#include <cassert>
#include <stdexcept>
#include <fstream>

#include <GL/glew.h>

namespace LGL {

    struct GLException : std::runtime_error
    {
        GLException( const char * msg )
        : std::runtime_error(msg)
        {

        }
    };

    struct GLProgram
    {
        GLProgram()
            : m_Program()
            , m_VertexShader()
            , m_FragmentShader()
        {
        }
        
        ~GLProgram()
        {
            glDeleteShader(m_VertexShader);
            m_VertexShader = 0;
            glDeleteShader(m_FragmentShader);
            m_FragmentShader = 0;
            glDeleteProgram(m_Program);
        }

        void LoadVertexShader(const char * path)
        {
            assert(m_VertexShader == 0);
            m_VertexShader = LoadShader(GL_VERTEX_SHADER, path);
        }

        void LoadFragmentShader(const char * path)
        {
            assert(m_FragmentShader == 0);
            m_FragmentShader = LoadShader(GL_FRAGMENT_SHADER, path);
        }

        void Use()
        {
            if (m_Program == 0) {
                assert(m_VertexShader != 0 && m_FragmentShader != 0);

                m_Program = glCreateProgram();
                glAttachShader(m_Program, m_VertexShader);
                glAttachShader(m_Program, m_FragmentShader);
                glLinkProgram(m_Program);

                GLint success;
                glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
                if (!success) {
                    GLchar msg[512];
                    glGetProgramInfoLog(m_Program, 512, NULL, msg);    

                    throw GLException(msg);
                }
            
            }

            glDeleteShader(m_VertexShader);
            m_VertexShader = 0;
            glDeleteShader(m_FragmentShader);
            m_FragmentShader = 0;

            glUseProgram(m_Program);
        }

        GLuint GetProgram() const
        {
            return m_Program;
        }

    private:
        GLuint LoadShader(GLenum shaderType, const char * path);

    private:
        GLuint m_Program;
        GLuint m_VertexShader;
        GLuint m_FragmentShader;
    };
}

#endif // #ifndef LGL_GLPROGRAM_H

