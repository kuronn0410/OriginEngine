#pragma once
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Renderer/Texture/TextureHandle.h"
namespace Game
{
	struct GameResources
	{
		MeshHandle playerMesh;
		MeshHandle enemyMesh;
		MeshHandle treeMesh;
		MeshHandle CubeMesh;

		TextureHandle backgroundTexture;
		TextureHandle baseTexture;
		TextureHandle groundTexture;
		TextureHandle rockTexture;
		TextureHandle clearTexture;
	};
}