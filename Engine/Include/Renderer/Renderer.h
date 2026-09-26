#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <dxcapi.h>

struct Vertex
{
    float x;
    float y;
    float z;
};

class ShaderCompiler;

class Renderer
{
public:
	bool Initialize(ShaderCompiler& shaderCompiler, ID3D12Device& device);
	void BeginFrame(ID3D12GraphicsCommandList* commandList);
	void Draw();
	
private:
	bool CreateRootSignature(ID3D12Device& device);
	bool CreatePipelineState(ID3D12Device& device, IDxcBlob& vertexShader, IDxcBlob& pixelShader);
	bool CreateVertexBuffer(ID3D12Device& device);

	Vertex vertices_[3] = {
		{ 0.0f, 0.5f, 0.0f },   // 上の頂点
		{ 0.5f, -0.5f, 0.0f },  // 右下の頂点
		{ -0.5f, -0.5f, 0.0f }  // 左下の頂点
	};// 三角形の頂点座標
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
};