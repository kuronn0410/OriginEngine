#pragma once
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Renderer/Texture/TextureHandle.h"
#include "include/Renderer/Object/Transform.h"
struct RenderObject
{
    MeshHandle mesh;
    TextureHandle texture;
    Transform transform;
};