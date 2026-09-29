#pragma once
#include "MeshHandle.h"
#include "Transform.h"
#include "include/Math/Matrix4x4.h"
class Object
{
public:
    /*初期化*/
    void SetMeshHandle(MeshHandle meshHandle);
    void SetTransform(const Transform& transform);

    /*取得*/
    const MeshHandle& GetMeshHandle() const;
    Transform& GetTransform();
    const Transform& GetTransform() const;
	Matrix4x4 GetWorldMatrix() const;
private:
    MeshHandle meshHandle_;
    Transform transform_;
};