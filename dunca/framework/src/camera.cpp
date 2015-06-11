#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(float x, float y, float w, float h,
       float fov, float aspect, float near, float far)
    : mViewport(x, y, w, h)
{
     mProjection = glm::perspective(fov, aspect, near, far);
}