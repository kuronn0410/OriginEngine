#include "include/Renderer/Camera/Camera.h"
#include "include/Math/MatrixMath.h"
#include "include/Math/Matrix4x4.h"
#include "include/Renderer/Camera/CameraTransform.h"

void Camera::SetTransform(const CameraTransform& transform)
{
	transform_ = transform;
}

Matrix4x4 Camera::GetViewMatrix() const
{
    Matrix4x4 cameraWorld =
        MakeTranslateMatrix(transform_.position);

    return MakeInverseMatrix(cameraWorld);
}