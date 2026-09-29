#include "include/Renderer/Object.h"
#include "include/Math/MatrixMath.h"
#include "include/Math/Matrix4x4.h"
#include "include/Renderer/MeshHandle.h"
#include "include/Renderer/Transform.h"

void Object::SetTransform(const Transform& transform)
{
    transform_ = transform;
}

void Object::SetMeshHandle(MeshHandle meshHandle)
{
    meshHandle_ = meshHandle;
}

const MeshHandle& Object::GetMeshHandle() const
{
    return meshHandle_;
}

Transform& Object::GetTransform()
{
    return transform_;
}

const Transform& Object::GetTransform() const
{
    return transform_;
}

Matrix4x4 Object::GetWorldMatrix() const
{

	Matrix4x4 translationMatrix = MakeTranslateMatrix(transform_.position);
	return translationMatrix;
}