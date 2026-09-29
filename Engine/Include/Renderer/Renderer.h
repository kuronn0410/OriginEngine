#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <dxcapi.h>
#include "include/Renderer/WorldTransform.h"
#include <cstdint>
#include <Windows.h>

class Mesh;
class ShaderCompiler;
class Object;
class ResourceManager;

class Renderer
{
public:
	bool Initialize(ShaderCompiler& shaderCompiler, ID3D12Device& device);
	void BeginFrame(ID3D12GraphicsCommandList* commandList);
	void Draw(const Object& object);
	void  SetResourceManager(ResourceManager& resourceManager)
	{
		resourceManager_ = &resourceManager;
	}
private:
	bool CreateRootSignature(ID3D12Device& device);
	bool CreatePipelineState(
		ID3D12Device& device, 
		IDxcBlob& vertexShader, 
		IDxcBlob& pixelShader
	);
	bool CreateWorldConstantBuffer(ID3D12Device& device);
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	ResourceManager* resourceManager_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> worldConstantBuffer_;
	WorldTransform* mappedWorldTransform_ = nullptr;
	uint32_t drawCount_ = 0;
	UINT alignedWorldTransformSize_ = 0;
};