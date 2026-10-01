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
    Matrix4x4 rotationMatrix =
        MakeRotateMatrix(transform_.rotation);

    Matrix4x4 translationMatrix =
        MakeTranslateMatrix(transform_.position);

    Matrix4x4 cameraWorld =
        Multiply(rotationMatrix, translationMatrix);


    return MakeInverseMatrix(cameraWorld);
}

Matrix4x4 Camera::GetProjectionMatrix() const
{
	return MakePerspectiveFovMatrix(
		projectionSettings_.fov,
		projectionSettings_.aspectRatio,
		projectionSettings_.nearPlane,
		projectionSettings_.farPlane
	);
}