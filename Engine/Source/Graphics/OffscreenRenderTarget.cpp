#include "Include/Graphics/OffscreenRenderTarget.h"

namespace Origin
{
	/*===========初期化==============*/
	bool OffscreenRenderTarget::Initialize(
		ID3D12Device& device, 
		UINT width, 
		UINT height, 
		const DescriptorAllocation& allocation
	)
	{
		if(!CreateColorTextureResource(device, width, height))
		{
			return false;
		}
		if(!CreateRTV(device))
		{
			return false;
		}
		if(!CreateSRV(device, allocation.cpuHandle, allocation.gpuHandle))
		{
			return false;
		}
		if(!CreateDepthTextureResource(device, width, height))
		{
			return false;
		}
		if(!CreateDSV(device))
		{
			return false;
		}
		return true;
	}

	bool OffscreenRenderTarget::CreateColorTextureResource(ID3D12Device& device,
		UINT width,
		UINT height)
	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 1;// 1つのGPUしか使わないので
		heapProperties.VisibleNodeMask = 1;	


		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = width;
		resourceDesc.Height = height;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色を保存する形式
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;//レンダーターゲットとして使うためのフラグ

		D3D12_CLEAR_VALUE clearValue{};
		clearValue.Format = resourceDesc.Format;
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 1.0f;
		// ④ Texture生成
		HRESULT hr = device.CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			&clearValue,
			IID_PPV_ARGS(colorTexture_.GetAddressOf())
		);
		if (FAILED(hr))
		{
			return false;
		}
		return true;
		
	}
	/*=============RTV生成===========*/
	bool OffscreenRenderTarget::CreateRTV(ID3D12Device& device)
	{

		if(!CreateRTVDescriptorHeap(device))
		{
			return false;
		}
		if(!CreateRenderTargetView(device))
		{
			return false;
		}	
		return true;
	}
	bool OffscreenRenderTarget::CreateRTVDescriptorHeap(ID3D12Device& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};

		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 0;

		HRESULT hr = device.CreateDescriptorHeap(
			&desc,
			IID_PPV_ARGS(rtvHeap_.GetAddressOf())
		);

		if (FAILED(hr))
		{
			return false;
		}
		return true;
	}
	bool OffscreenRenderTarget::CreateRenderTargetView(ID3D12Device& device)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle =
			rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		device.CreateRenderTargetView(
			colorTexture_.Get(),
			nullptr,
			rtvHandle
		);
		return true;
	}
	/*========================*/
	bool OffscreenRenderTarget::CreateSRV(ID3D12Device& device,
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = colorTexture_->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Texture2D.MipLevels = colorTexture_->GetDesc().MipLevels;

		device.CreateShaderResourceView(
			colorTexture_.Get(),
			&srvDesc,
			cpuHandle
		);
		gpuHandle_ = gpuHandle;
		return true;
	}
	bool OffscreenRenderTarget::CreateDepthTextureResource(
		ID3D12Device& device,
		UINT width,
		UINT height)
	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 1;// 1つのGPUしか使わないので
		heapProperties.VisibleNodeMask = 1;


		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = width;
		resourceDesc.Height = height;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//32bitの浮動小数点数で深度値を保存する形式
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE clearValue{};
		clearValue.Format = resourceDesc.Format;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;
		// ④ Texture生成
		HRESULT hr = device.CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度バッファへの書き込みに対応した状態で生成するため
			&clearValue,
			IID_PPV_ARGS(depthTexture_.GetAddressOf())
		);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}
	/*============DSV生成============*/
	bool OffscreenRenderTarget::CreateDSV(ID3D12Device& device)
	{
		if (!CreateDSVDescriptorHeap(device))
		{
			return false;
		}
		if (!CreateDepthStencilView(device))
		{
			return false;
		}
		return true;
	}
	bool OffscreenRenderTarget::CreateDSVDescriptorHeap(ID3D12Device& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};

		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 0;

		HRESULT hr = device.CreateDescriptorHeap(
			&desc,
			IID_PPV_ARGS(dsvHeap_.GetAddressOf())
		);

		if (FAILED(hr))
		{
			return false;
		}
		return true;
	}
	bool OffscreenRenderTarget::CreateDepthStencilView(ID3D12Device& device)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle =
			dsvHeap_->GetCPUDescriptorHandleForHeapStart();
		device.CreateDepthStencilView(
			depthTexture_.Get(),
			nullptr,
			rtvHandle
		);
		return true;
	}
	/*========================*/


}