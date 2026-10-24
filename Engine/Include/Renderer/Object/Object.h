#pragma once
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Renderer/Object/Transform.h"

#include "include/Renderer/Texture/TextureHandle.h"
#include "include/Math/Matrix4x4.h"
#include "include/Renderer/Object/RenderObject.h"

namespace Origin
{
    class Object
    {
    public:
        /*初期化*/
        void SetRenderObject(const RenderObject& renderObject);

        /*取得*/
        Matrix4x4 GetWorldMatrix() const;

        const RenderObject& GetRenderObject()const;
        RenderObject& GetRenderObject();
    private:
        RenderObject renderObject_;
    };
}
