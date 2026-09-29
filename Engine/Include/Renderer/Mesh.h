#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include "include/Renderer/MeshData.h"
#include <Windows.h>

class Mesh
{
public:
	bool CreateBuffer(ID3D12Device& device, const MeshData& meshData);
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const { return vertexBufferView_; }

	const UINT GetVertexCount() const { return vertexCount_; }
	const  UINT GetIndexCount() const { return indexCount_; }
	const D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const { return indexBufferView_; }
private:
	bool CreateVertexBuffer(ID3D12Device& device, const MeshData& meshData);
	bool CreateIndexBuffer(ID3D12Device& device, const MeshData& meshData);
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	UINT indexCount_ = 0;
	UINT vertexCount_ = 0;

};