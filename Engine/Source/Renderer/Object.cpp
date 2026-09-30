#include "include/Renderer/Object/Object.h"
#include "include/Math/MatrixMath.h"
#include "include/Math/Matrix4x4.h"
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Renderer/Object/Transform.h"

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
	Matrix4x4 scaleMatrix = MakeScaleMatrix(transform_.scale);
	Matrix4x4 rotationMatrix = MakeRotateMatrix(transform_.rotation);
    Matrix4x4 world = Multiply(scaleMatrix, rotationMatrix);
	world = Multiply(world, translationMatrix);
	return world;
}

