#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include "include/Renderer/Texture/TextureData.h"
#include "include/Renderer/Texture/TextureUploadData.h"	
class Texture
{
public:

	bool Initialize(
		ID3D12Device& device,
		const TextureData& data,
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
		ID3D12GraphicsCommandList& commandList
	);
	ID3D12Resource* GetResource() const
	{
		return textureResource_.Get();
	}

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const
	{
		return gpuHandle_;
	}
private:
	bool CreateTextureResource(
		ID3D12Device& device, 
		const TextureData& data
	);

	bool CreateUploadBuffer(
		ID3D12Device& device
	);
	bool UploadTextureData(
		ID3D12GraphicsCommandList& commandList,
		const TextureData& data
	);

	void CreateSRV(
		ID3D12Device& device,
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle
	);
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_{};
	TextureUploadData uploadData_{};
};