

#ifndef LGL_VERTEXARRAYOBJECT_H
#define LGL_VERTEXARRAYOBJECT_H 1

#include <GL/glew.h>

namespace LGL {

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
                glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_ENTRY)* count, buffer, GL_STATIC_DRAW);
            }

            m_BufferObjectAddress = buffer;
            m_Count = count;

            // !TODO
            // map types and sizes from ELEMENT in a generic way

            const GLuint elementSize = sizeof(ELEMENT) / sizeof(GLfloat);
            const GLuint strideSize = sizeof(VERTEX_ENTRY);
            const GLvoid * relativeOffset = static_cast<const GLvoid*>(&(static_cast<VERTEX_ENTRY*>(0)->*offset));
            glVertexAttribPointer(location, elementSize, GL_FLOAT, GL_FALSE, strideSize, relativeOffset);
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

} // namespace LGL

#endif // #ifndef LGL_VERTEXARRAYOBJECT_H