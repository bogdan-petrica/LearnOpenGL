#pragma once
#include "ICamera.h"

class FPSCamera : ICamera
{
public:
    FPSCamera( const glm::vec3& aUp, const glm::vec3& aCameraLocation, const glm::vec3& aTargetDirection );
    virtual void updateMovement( ICamera::Camera_Movement aDirection, float aDeltaTime );
    virtual void updateRotation( float aPitch, float aYaw );
    virtual glm::mat4 getViewMatrix();
protected:
    virtual void computeViewMatrix( const glm::vec3& aUp, const glm::vec3& aCameraLocation, const glm::vec3& aTargetDirection );
    glm::mat4 mViewMat;
private:
    glm::vec3 mUp;
    glm::vec3 mCamPos;
    glm::vec3 mDirection;
    float mMovementSpeed;
    float mCurrentPitch;
    float mCurrentYaw;
    bool sholdRecompute;
};

FPSCamera::FPSCamera( const glm::vec3& aUp, const glm::vec3& aCameraLocation, const glm::vec3& aTargetDirection )
    : mUp( aUp )
    , mCamPos( aCameraLocation )
    , mDirection( aTargetDirection )
    , mMovementSpeed( 2.0f )
    , mCurrentPitch( 45.0f )
    , mCurrentYaw( 0.0f )
    , sholdRecompute( true )
{
    computeViewMatrix( aUp, aCameraLocation, aTargetDirection );
}

void FPSCamera::computeViewMatrix( const glm::vec3& aUp, const glm::vec3& aCameraLocation, const glm::vec3& aTargetDirection )
{
    mViewMat = glm::lookAt( aCameraLocation, aTargetDirection, aUp );
    sholdRecompute = false;
}

void FPSCamera::updateMovement( ICamera::Camera_Movement aDirection, float aDeltaTime )
{
    float amount = mMovementSpeed * aDeltaTime;
    switch( aDirection )
    {
    case ICamera::FORWARD:
        {
            mCamPos += mDirection * amount;
            break;
        }
    case ICamera::BACKWARD:
        {
            mCamPos -= mDirection * amount;
            break;
        }
    case ICamera::LEFT:
        {
            glm::vec3 tempRightVec( mViewMat[0] );
            mCamPos -= tempRightVec * amount;
            break;
        }
    case ICamera::RIGHT:
        {
            glm::vec3 tempRightVec( mViewMat[0] );
            mCamPos += tempRightVec * amount;
            break;
        }
    default:
        break;
    }
    sholdRecompute = true;
}

void FPSCamera::updateRotation( float aPitch, float aYaw )
{
    const float mouseSensitivity = 0.25;
    aPitch *= mouseSensitivity;
    aYaw *= mouseSensitivity;
    mCurrentPitch += aPitch;
    mCurrentYaw += aYaw;
    if( mCurrentPitch > 89.0 )
    {
        mCurrentPitch = 89.0;
    }
    else if( mCurrentPitch < -89.0 )
    {
        mCurrentPitch = -89.0;
    }
    glm::vec3 front;
    front.x = cos( glm::radians( mCurrentYaw ) ) * cos(glm::radians( mCurrentPitch ) );
    front.y = sin( glm::radians( mCurrentPitch ) );
    front.z = sin( glm::radians( mCurrentYaw ) ) * cos(glm::radians( mCurrentPitch ) );
    mDirection = mCamPos + glm::normalize( front );

    sholdRecompute = true;

}

glm::mat4 FPSCamera::getViewMatrix()
{
    if( sholdRecompute )
    {
        computeViewMatrix( mUp, mCamPos, mDirection );
    }
    return mViewMat;
}