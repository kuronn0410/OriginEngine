#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include "include/Renderer/MeshData.h"
#include "include/Math/Vector3.h"
#include "include/Math/Matrix4x4.h"

class Mesh
{
public:
	bool CreateVertexBuffer(ID3D12Device& device, MeshData meshData);
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const { return vertexBufferView_; }
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
};