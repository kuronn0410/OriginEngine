#include "include/Renderer/Texture/Texture.h"
#include <d3d12.h>
#include "include/Renderer/Texture/TextureData.h"
#include "include/Renderer/Texture/TextureUploadData.h"
#include <Windows.h>
#include <string.h> 
#include <cstddef>
bool Texture::Initialize(
	ID3D12Device& device,
	const TextureData& data,
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
	ID3D12GraphicsCommandList& commandList
)
{
	if (!CreateTextureResource(device, data))
	{
		return false;
	}
	if (!CreateUploadBuffer(device))
	{
		return false;
	}
	if (!UploadTextureData(commandList, data))
	{
		return false;
	}

	CreateSRV(device, cpuHandle, gpuHandle);

	return true;
}


bool Texture::CreateTextureResource(
	ID3D12Device& device,
	const TextureData& data
)
{

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = data.width;
	resourceDesc.Height = data.height;
	resourceDesc.Format = data.format;

	HRESULT hr = device.CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&textureResource_)
	);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}



bool Texture::CreateUploadBuffer(
	ID3D12Device& device
)
{
   // textureResource_ の情報から
   
   // Upload Bufferに必要なサイズを求める
	const D3D12_RESOURCE_DESC textureDesc =
		textureResource_->GetDesc();

	UINT64 uploadBufferSize = 0;

	device.GetCopyableFootprints(
		&textureDesc,
		0,
		1,
		0,
		&uploadData_.footprint,
		nullptr,
		nullptr,
		&uploadBufferSize
	);

   // UPLOAD Heap を作る
	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
   // BUFFER用の D3D12_RESOURCE_DESC を作る
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = uploadBufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_GENERIC_READ;
   // CreateCommittedResource()
	HRESULT hr = device.CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		initialState,
		nullptr,
		IID_PPV_ARGS(&uploadData_.uploadBuffer)
	);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Texture::UploadTextureData(
	ID3D12GraphicsCommandList& commandList,
	const TextureData& data
)
{
	void* mappedData = nullptr;
	HRESULT hr = uploadData_.uploadBuffer->Map(
		0, 
		nullptr, 
		&mappedData
	);

	if (FAILED(hr))
	{
		return false;
	}

	const size_t srcRowSize =
		static_cast<size_t>(data.width) * 4;

	const size_t dstRowPitch =
		uploadData_.footprint.Footprint.RowPitch;

	std::byte* dst = static_cast<std::byte*>(mappedData) + uploadData_.footprint.Offset;

	const std::byte* src = data.pixels.data();

	for (UINT y = 0; y < data.height; ++y)
	{
		std::memcpy(
			dst + y * dstRowPitch,
			src + y * srcRowSize,
			srcRowSize
		);
	}
	uploadData_.uploadBuffer->Unmap(0, nullptr);

	D3D12_TEXTURE_COPY_LOCATION srcLocation{};
	srcLocation.pResource = uploadData_.uploadBuffer.Get();
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	srcLocation.PlacedFootprint = uploadData_.footprint;

	D3D12_TEXTURE_COPY_LOCATION dstLocation{};
	dstLocation.pResource = textureResource_.Get();
	dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLocation.SubresourceIndex = 0;

	commandList.CopyTextureRegion(
		&dstLocation, 
		0,
		0, 
		0, 
		&srcLocation, 
		nullptr
	);
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	barrier.Transition.pResource = textureResource_.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList.ResourceBarrier(
		1,
		&barrier
	);


	return true;
}

void Texture::CreateSRV(
	ID3D12Device& device,
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle
)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureResource_->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = textureResource_->GetDesc().MipLevels;

	device.CreateShaderResourceView(
		textureResource_.Get(),
		&srvDesc,
		cpuHandle
	);
	gpuHandle_ = gpuHandle;
}