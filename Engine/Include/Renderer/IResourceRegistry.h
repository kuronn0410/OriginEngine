#pragma once
#include "MeshHandle.h"
#include "MeshData.h"
class IResourceRegistry
{
public:
    virtual ~IResourceRegistry() = default;

    virtual MeshHandle RegisterMesh(
        const MeshData& meshData
    ) = 0;
};