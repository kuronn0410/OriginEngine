#pragma once

#include "include/Math/Matrix4x4.h"
#include "include/Renderer/Camera/CameraTransform.h"

class Camera
{
public:
    void SetTransform(const CameraTransform& transform);
    Matrix4x4 GetViewMatrix() const;

    CameraTransform& GetTransform();
    const CameraTransform& GetTransform() const;
private:
    CameraTransform transform_;
};