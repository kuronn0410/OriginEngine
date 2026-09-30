#include "Include/Renderer/Mesh/Mesh.h"
#include <d3d12.h>
#include <cstring>
#include "Include/Renderer/Mesh/Vertex.h"
#include <wrl/client.h>
#include <Windows.h>
#include "include/Renderer/Mesh/MeshData.h"
#include <dxgiformat.h>





bool Mesh::CreateBuffer(ID3D12Device& device, const MeshData& meshData)
{
	if (!CreateVertexBuffer(device, meshData))
	{
		return false;
	}
	if (!CreateIndexBuffer(device, meshData))
	{
		return false;
	}
	return true;
}

bool Mesh::CreateVertexBuffer(ID3D12Device& device, const MeshData& meshData)
{
	//①受け取ったMeshDataをから必要なデータを取得
    const Vertex* vertices = meshData.vertices.data();//保持するか考える
    vertexCount_ = static_cast<UINT>(meshData.vertices.size());

    // ② 必要なバッファサイズを計算
    const UINT vertexBufferSize = sizeof(Vertex) * vertexCount_;

    // ③ VertexBuffer用のResourceを生成
    D3D12_HEAP_PROPERTIES heapProperties = {};
    heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = vertexBufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_GENERIC_READ;

    HRESULT hr = device.CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        initialState,
        nullptr,
        IID_PPV_ARGS(vertexBuffer_.GetAddressOf())
    );
    if (FAILED(hr))
    {
        return false;
    }

    // ④ Map
    void* mappedData = nullptr;

    hr = vertexBuffer_->Map(
        0,
        nullptr,
        &mappedData
    );

    if (FAILED(hr))
    {
        return false;
    }

    // ⑤ verticesをコピー
    //std::memcpy(mappedData, vertices, sizeof(Vertex) * vertexCount);
    std::memcpy(mappedData, vertices, vertexBufferSize);
    // ⑥ Unmap
    vertexBuffer_->Unmap(0, nullptr);

    // ⑦ VertexBufferView
    vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = vertexBufferSize;
    vertexBufferView_.StrideInBytes = sizeof(Vertex);
    return true;
}



bool Mesh::CreateIndexBuffer(ID3D12Device& device, const MeshData& meshData)
{
    if (meshData.indices.empty())
    {
        return false;
    }
    //①受け取ったMeshDataをから必要なデータを取得
    const uint32_t* indices = meshData.indices.data();//保持するか考える
    indexCount_ = static_cast<UINT>(meshData.indices.size());
    // ② 必要なバッファサイズを計算
    const UINT indexBufferSize = sizeof(uint32_t) * indexCount_;

    // ③ IndexBuffer用のResourceを生成
    D3D12_HEAP_PROPERTIES heapProperties = {};
    heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = indexBufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_GENERIC_READ;

    HRESULT hr = device.CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        initialState,
        nullptr,
        IID_PPV_ARGS(indexBuffer_.GetAddressOf())
    );
    if (FAILED(hr))
    {
        return false;
    }

    // ④ Map
    void* mappedData = nullptr;

    hr = indexBuffer_->Map(
        0,
        nullptr,
        &mappedData
    );

    if (FAILED(hr))
    {
        return false;
    }

    // ⑤ indicesをコピー
    //std::memcpy(mappedData, indices, sizeof(UINT) * indexCount);
    std::memcpy(mappedData, indices, indexBufferSize);
    // ⑥ Unmap
    indexBuffer_->Unmap(0, nullptr);

    // ⑦ IndexBufferView
    indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = indexBufferSize;
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
    return true;
}