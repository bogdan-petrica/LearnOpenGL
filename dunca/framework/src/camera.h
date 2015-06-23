#ifndef SD_FW_CAMERA_H
#define SD_FW_CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(int x, int y, int w, int h,
        float fov, float aspect, float near, float far);

    const glm::ivec4&
    viewport() const;

    const glm::mat4&
    projection() const;

    const glm::mat4&
    view() const;

    void
    setPos(glm::vec3 pos);
    const glm::vec3&
    pos() const;

    void
    setLookAt(glm::vec3 target);
    const glm::vec3&
    lookAt() const;

private:
    void
    computeViewMatrix(glm::mat4& viewMat) const;

private:
    glm::ivec4 mViewport;

    glm::mat4 mProjection;

    mutable glm::mat4 mView;
    mutable bool mDirty;

    glm::vec3 mPos;
    glm::vec3 mTarget;
};

//******************************************************************************
inline const glm::ivec4&
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

//******************************************************************************
inline const glm::mat4&
Camera::view() const
{
    if(mDirty)
    {
        computeViewMatrix(mView);
        mDirty = false;
    }
    return mView;
}

//******************************************************************************
inline void
Camera::setPos(glm::vec3 pos)
{
    if(pos != mPos)
    {
        mDirty = true;
        mPos = pos;
    }
}

//******************************************************************************
inline const glm::vec3&
Camera::pos() const
{
    return mPos;
}

//******************************************************************************
inline void
Camera::setLookAt(glm::vec3 target)
{
    if(target != mTarget)
    {
        mDirty = true;
        mTarget = target;
    }
}

//******************************************************************************
inline const glm::vec3&
Camera::lookAt() const
{
    return mTarget;
}

#endif