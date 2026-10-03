#pragma once
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Renderer/Texture/TextureHandle.h"
struct GameResources
{
    MeshHandle playerMesh;
    MeshHandle enemyMesh;
    MeshHandle treeMesh;

	TextureHandle backgroundTexture;
};