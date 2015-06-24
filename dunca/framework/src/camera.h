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

    void
    setAspectRatio(float value);
    float
    aspectRatio() const;

    void
    setFov(float value);
    float
    fov() const;

    const glm::vec3&
    getUpVec() const;

private:
    void
    computeViewMatrix(glm::mat4& viewMat) const;

private:
    glm::ivec4 mViewport;

    mutable glm::mat4 mProjection;
    mutable bool mProjDirty;
    float mFov;
    float mAspect;
    float mNear;
    float mFar;

    mutable glm::mat4 mView;
    mutable bool mViewDirty;

    glm::vec3 mPos;
    glm::vec3 mTarget;

    static const glm::vec3 mCameraUpVec;
};

//******************************************************************************
inline const glm::ivec4&
Camera::viewport() const
{
    return mViewport;
}

//******************************************************************************
inline const glm::mat4&
Camera::view() const
{
    if(mViewDirty)
    {
        computeViewMatrix(mView);
        mViewDirty = false;
    }
    return mView;
}

//******************************************************************************
inline void
Camera::setPos(glm::vec3 pos)
{
    if(pos != mPos)
    {
        mViewDirty = true;
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
        mViewDirty = true;
        mTarget = target;
    }
}

//******************************************************************************
inline const glm::vec3&
Camera::lookAt() const
{
    return mTarget;
}

//******************************************************************************
inline void
Camera::setAspectRatio(float value)
{
    if(mAspect != value)
    {
        mAspect = value;
        mProjDirty = true;
    }
}

//******************************************************************************
inline float
Camera::aspectRatio() const
{
    return mAspect;
}

//******************************************************************************
inline void
Camera::setFov(float value)
{
    if(mFov != value)
    {
        mFov = value;
        mProjDirty = true;
    }
}

//******************************************************************************
inline float
Camera::fov() const
{
    return mFov;
}

//******************************************************************************
inline const glm::vec3&
Camera::getUpVec() const
{
    return mCameraUpVec;
}

#endif