#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*static*/ const glm::vec3 Camera::mCameraUpVec(0.0f, 1.0f, 0.0f);

Camera::Camera(int x, int y, int w, int h,
       float fov, float aspect, float near, float far)
    : mViewport(x, y, w, h)
    , mProjDirty(true)
    , mFov(fov)
    , mAspect(aspect)
    , mNear(near)
    , mFar(far)
    , mViewDirty(true)
    , mPos(0.0f, 0.0f, 0.0f)
    , mTarget(0.0f, 0.0f, 0.0f)
{
}

//******************************************************************************
const glm::mat4&
Camera::projection() const
{
    if(mProjDirty)
    {
        mProjection = glm::perspective(mFov, mAspect, mNear, mFar);
        mProjDirty = false;
    }
    return mProjection;
}

//******************************************************************************
void
Camera::computeViewMatrix(glm::mat4& viewMat) const
{
    // Pointing in the inverse direction of the camera (we move the world not the camera)
    // TODO: Check why doesn't work at all
    /*
    glm::vec3 direction = glm::normalize(mPos - mTarget);
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
    glm::vec3 up = glm::cross(direction, right);

    glm::mat4 leftMat(glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(direction, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    glm::mat4 rightMat;
    rightMat[0][3] = -mPos.x; rightMat[1][3] = -mPos.y; rightMat[2][3] = -mPos.z;

    viewMat = leftMat * rightMat;
    */
    viewMat = glm::lookAt(mPos, mTarget, mCameraUpVec);
}

