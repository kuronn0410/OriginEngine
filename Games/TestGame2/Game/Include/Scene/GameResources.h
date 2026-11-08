#pragma once
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Renderer/Texture/TextureHandle.h"
namespace Game
{
	// ゲーム内で使用するリソースをまとめた構造体
	struct GameResources
	{
		Origin::MeshHandle playerMesh;
		Origin::MeshHandle enemyMesh;
		Origin::MeshHandle treeMesh;
		Origin::MeshHandle CubeMesh;

		Origin::TextureHandle backgroundTexture;
		Origin::TextureHandle baseTexture;
		Origin::TextureHandle groundTexture;
		Origin::TextureHandle rockTexture;
		Origin::TextureHandle clearTexture;
	};
}