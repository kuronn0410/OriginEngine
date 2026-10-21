#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/Renderer/Mesh/MeshHandle.h"
#include "Include/Renderer/Mesh/MeshData.h"
#include "Include/Renderer/Mesh/Mesh.h"
#include "Include/Renderer/Texture/TextureHandle.h"
#include "Include/Renderer/Texture/Texture.h"
#include "Include/Renderer/Texture/TextureLoader.h"
#include "Include/Renderer/Texture/TextureData.h"
#include "Include/Graphics/DescriptorAllocation.h"
#include <memory>
#include <cstdint>
#include <utility>
#include <d3d12.h>
#include <string>
#include <vector>
#include <Windows.h>
MeshHandle ResourceManager::RegisterMesh(
	const MeshData& meshData
	)
{

	auto mesh = std::make_unique<Mesh>();

	if (!mesh->CreateBuffer(device_, meshData))
	{
		// 登録失敗
		return MeshHandle{};
	}

	meshes_.push_back(std::move(mesh));

	return MeshHandle{
		static_cast<uint32_t>(meshes_.size() - 1)
	};
}

TextureHandle ResourceManager::RegisterTexture(
	const std::string& path
)
{
	TextureLoader loader;
	TextureData textureData = loader.Load(path);
	auto texture = std::make_unique<Texture>();
	uint32_t index = static_cast<uint32_t>(textures_.size());
	
	DescriptorAllocation descriptorAllocation = descriptorAllocator_.Allocate();

	if (!texture->Initialize(device_, 
		textureData, descriptorAllocation.cpuHandle, 
		descriptorAllocation.gpuHandle, 
		commandList_))
	{
		// 登録失敗
		return TextureHandle{};
	}

	textures_.push_back(std::move(texture));

	return TextureHandle{
		static_cast<uint32_t>(textures_.size() - 1)
	};
}



