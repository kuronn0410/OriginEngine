#pragma once
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Renderer/Mesh/MeshData.h"
namespace Origin
{ 
    class IResourceRegistry
    {
    public:
        virtual ~IResourceRegistry() = default;

        virtual MeshHandle RegisterMesh(
            const MeshData& meshData
        ) = 0;
    };
}