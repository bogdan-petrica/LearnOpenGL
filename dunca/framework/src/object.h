#if !defined SD_FW_OBJECT_H
#define SD_FW_OBJECT_H

#include <glm/glm.hpp>

class Object
{
public:
    Object();

    virtual void
    computeModelMatrix(glm::mat4& m);

    virtual void
    prepare() {};

    glm::vec3 rotate;
    glm::vec3 translate;
    glm::vec3 scale;
};

#endif // #ifdef SD_FW_OBJECT_H