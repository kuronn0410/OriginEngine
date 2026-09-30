#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/Renderer/Mesh/MeshHandle.h"
#include "Include/Renderer/Mesh/MeshData.h"
#include "Include/Renderer/Mesh/Mesh.h"
#include <memory>
#include <cstdint>
#include <utility>

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

