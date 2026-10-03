#pragma once
#include "IResourceRegistry.h"
#include "include/Renderer/Mesh/MeshData.h"
#include "include/Renderer/Mesh/MeshHandle.h" 
#include "include/Renderer/Mesh/Mesh.h"
#include "include/Renderer/Texture/TextureHandle.h" 
#include "include/Renderer/Texture/Texture.h"
#include <vector>
#include <memory>
#include <D3D12.h>
#include <string>
#include <Windows.h> 
/// <summary>
/// リソースを管理するクラス
/// </summary>
class ResourceManager : public IResourceRegistry
{
public:
	ResourceManager(
		ID3D12Device& device,
		ID3D12GraphicsCommandList& commandList,
		ID3D12DescriptorHeap& srvHeap,
		UINT srvDescriptorSize,
		UINT maxSrvCount): 

        device_(device),
        commandList_(commandList),
        srvHeap_(srvHeap),
        srvDescriptorSize_(srvDescriptorSize),
        maxSrvCount_(maxSrvCount)

    {
    }
	/*-- メッシュ --*/
    MeshHandle RegisterMesh(
        const MeshData& meshData
    ) override;

	const Mesh* GetMesh(const MeshHandle& meshHandle) const
	{
		if (meshHandle.id < meshes_.size())
		{
			return meshes_[meshHandle.id].get();
		}
		return nullptr;
	}

	/*-- テクスチャ --*/
	TextureHandle RegisterTexture(
		const std::string& path
	);

	const Texture* GetTexture(const TextureHandle& textureHandle) const
	{
		if (textureHandle.id < textures_.size())
		{
			return textures_[textureHandle.id].get();
		}
		return nullptr;
	}

	ID3D12DescriptorHeap* GetSRVHeap() const
	{
		return &srvHeap_;
	}


private:
    // MeshDataを反映して生成されたMeshを所有
    std::vector<std::unique_ptr<Mesh>> meshes_;
	std::vector<std::unique_ptr<Texture>> textures_;


    ID3D12Device& device_;
	ID3D12GraphicsCommandList& commandList_;
	ID3D12DescriptorHeap& srvHeap_;
	UINT srvDescriptorSize_;
	UINT maxSrvCount_ = 256;
};