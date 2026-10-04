#include "include/Renderer/Object/Object.h"
#include "include/Math/MatrixMath.h"
#include "include/Math/Matrix4x4.h"
#include "include/Renderer/Object/Transform.h"

void Object::SetRenderObject(const RenderObject& renderObject)
{
	renderObject_ = renderObject;
}

const RenderObject& Object::GetRenderObject() const
{
	return renderObject_;
}

RenderObject& Object::GetRenderObject()
{
	return renderObject_;
}
Matrix4x4 Object::GetWorldMatrix() const
{

	Matrix4x4 translationMatrix = MakeTranslateMatrix(renderObject_.transform.position);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(renderObject_.transform.scale);
	Matrix4x4 rotationMatrix = MakeRotateMatrix(renderObject_.transform.rotation);
    Matrix4x4 world = Multiply(scaleMatrix, rotationMatrix);
	world = Multiply(world, translationMatrix);
	return world;
}

