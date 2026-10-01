#pragma once

#include "include/Math/Matrix4x4.h"
#include "include/Renderer/Camera/CameraTransform.h"
#include "include/Renderer/Camera/ProjectionSettings.h"
class Camera
{
public:
    void SetTransform(const CameraTransform& transform);
	void SetProjectionSettings(const ProjectionSettings& settings)
	{
		projectionSettings_ = settings;
	}
    Matrix4x4 GetViewMatrix() const;
    Matrix4x4 GetProjectionMatrix() const;
    CameraTransform& GetTransform()
    {
		return transform_;
    }
    const CameraTransform& GetTransform() const;
private:
    CameraTransform transform_;
	ProjectionSettings projectionSettings_;
};