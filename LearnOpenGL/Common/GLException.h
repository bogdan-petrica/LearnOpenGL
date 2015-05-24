#ifndef LGL_GLEXCEPTION_H
#define LGL_GLEXCEPTION_H 1

#include <stdexcept>

namespace LGL {

    struct GLException : std::runtime_error
    {
        GLException(const char * msg)
        : std::runtime_error(msg)
        {

        }
    };

} // namespace LGL

#endif // #ifndef LGL_GLEXCEPTION_H