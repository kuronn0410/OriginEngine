#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/Renderer/Mesh/MeshHandle.h"
#include "Include/Renderer/Mesh/MeshData.h"
#include "Include/Renderer/Mesh/Mesh.h"
#include "Include/Renderer/Texture/TextureHandle.h"
#include "Include/Renderer/Texture/Texture.h"
#include "Include/Renderer/Texture/TextureLoader.h"
#include "Include/Renderer/Texture/TextureData.h"
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

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle =
		srvHeap_.GetGPUDescriptorHandleForHeapStart();

	gpuHandle.ptr +=
		static_cast<UINT64>(index) * srvDescriptorSize_;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle =
		srvHeap_.GetCPUDescriptorHandleForHeapStart();

	cpuHandle.ptr +=
		static_cast<SIZE_T>(index) * srvDescriptorSize_;

	if (!texture->Initialize(device_, textureData, cpuHandle, gpuHandle, commandList_))
	{
		// 登録失敗
		return TextureHandle{};
	}

	textures_.push_back(std::move(texture));

	return TextureHandle{
		static_cast<uint32_t>(textures_.size() - 1)
	};
}



