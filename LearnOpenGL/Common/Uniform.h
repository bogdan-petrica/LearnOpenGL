#ifndef LGL_UNIFORM_H
#define LGL_UNIFORM_H 1

#include "GLProgram.h"

namespace LGL {

    struct Uniform
    {
        Uniform(GLProgram & program, const char * name)
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


} // namespace LGL

#endif // #ifndef LGL_UNIFORM_H