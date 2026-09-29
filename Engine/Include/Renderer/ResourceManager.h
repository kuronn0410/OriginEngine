#pragma once
#include "IResourceRegistry.h"
#include "MeshData.h"
#include "MeshHandle.h" 
#include "Mesh.h"
#include <vector>
#include <memory>
#include <D3D12.h>
/// <summary>
/// リソースを管理するクラス
/// </summary>
class ResourceManager : public IResourceRegistry
{
public:
	ResourceManager(ID3D12Device& device) : 
        device_(device)
    {
    }
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
private:
    // MeshDataを反映して生成されたMeshを所有
    std::vector<std::unique_ptr<Mesh>> meshes_;
    ID3D12Device& device_;
};