#ifndef SD_FW_CAMERA_H
#define SD_FW_CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(float x, float y, float w, float h,
        float fov, float aspect, float near, float far);

    const glm::vec4&
    viewport() const;

    const glm::mat4&
    projection() const;

private:
    glm::vec4 mViewport;

    glm::mat4 mProjection;
};

//******************************************************************************
inline const glm::vec4&
Camera::viewport() const
{
    return mViewport;
}

//******************************************************************************
inline const glm::mat4&
Camera::projection() const
{
    return mProjection;
}

#endif