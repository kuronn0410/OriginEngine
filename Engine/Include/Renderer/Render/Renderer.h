#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <dxcapi.h>
#include "include/Renderer/Render/WorldTransform.h"
#include "include/Renderer/Camera/Camera.h"
#include <cstdint>
#include <Windows.h>
#include "include/Renderer/Object/Object.h"



namespace Origin
{
	class ResourceManager;
	class ShaderCompiler;
	class Object;
	class Camera;
}

namespace Origin
{
	class Renderer
	{
	public:
		bool Initialize(ShaderCompiler& shaderCompiler, ID3D12Device& device);
		void BeginFrame(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvHeap);
		void Draw(const Object& object);
		void  SetResourceManager(Origin::ResourceManager& resourceManager)
		{
			resourceManager_ = &resourceManager;
		}
		void SetCamera(const Camera& camera)
		{
			camera_ = &camera;
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
		ID3D12DescriptorHeap* srvHeap_ = nullptr;
		Origin::ResourceManager* resourceManager_ = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> worldConstantBuffer_;
		WorldTransform* mappedWorldTransform_ = nullptr;
		uint32_t drawCount_ = 0;
		UINT alignedWorldTransformSize_ = 0;

		// カメラの参照を保持するためのポインタ
		const Camera* camera_ = nullptr;
	};
}