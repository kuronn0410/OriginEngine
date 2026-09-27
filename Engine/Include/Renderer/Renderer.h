#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <dxcapi.h>

class Mesh;
class ShaderCompiler;

class Renderer
{
public:
	bool Initialize(ShaderCompiler& shaderCompiler, ID3D12Device& device);
	void BeginFrame(ID3D12GraphicsCommandList* commandList);
	void Draw(Mesh& mesh);
	
private:
	bool CreateRootSignature(ID3D12Device& device);
	bool CreatePipelineState(ID3D12Device& device, IDxcBlob& vertexShader, IDxcBlob& pixelShader);
	bool CreateVertexBuffer(ID3D12Device& device);
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	ID3D12GraphicsCommandList* commandList_ = nullptr;
};