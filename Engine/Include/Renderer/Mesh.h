#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include "Vertex.h"
#include <Windows.h>
class Mesh
{
public:
	bool CreateVertexBuffer(ID3D12Device& device, const Vertex* vertices, UINT vertexCount);
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const { return vertexBufferView_; }
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
};