#pragma once
#include <glm.hpp>
class ICamera
{
public:
    enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
    virtual ~ICamera(){};
    virtual void updateMovement( Camera_Movement aDirection, float aDeltaTime ) = 0;
    virtual void updateRotation( float aPitch, float aYaw ) = 0;
    virtual glm::mat4 getViewMatrix() = 0;
};