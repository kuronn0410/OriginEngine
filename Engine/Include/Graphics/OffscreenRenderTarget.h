#pragma once
#include "Include/Graphics/DescriptorAllocator.h"
#include <Windows.h>
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <dxgi1_5.h>
#include <d3d12.h>
namespace Origin
{
	class OffscreenRenderTarget
	{
	public:
		OffscreenRenderTarget(DescriptorAllocator& descriptorAllocator) : 
			descriptorAllocator_(descriptorAllocator) {}
		bool Initialize(ID3D12Device& device,
			UINT width,
			UINT height,
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle
		);
	private:
		bool CreateColorTextureResource(
			ID3D12Device& device,
			UINT width,
			UINT height
		);//Color Texture生成
		//RTV生成
		bool CreateRTV(ID3D12Device& device);
		bool CreateRTVDescriptorHeap(ID3D12Device& device);	
		bool CreateRenderTargetView(ID3D12Device& device);
		//SRV生成
		bool CreateSRV(
			ID3D12Device& device,
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle
		);
		bool CreateDepthTextureResource(ID3D12Device& device,
			UINT width,
			UINT height);
		bool CreateDSV(ID3D12Device& device);	
		bool CreateDSVDescriptorHeap(ID3D12Device& device);
		bool CreateDepthStencilView(ID3D12Device& device);
		// GraphicsのSRVはAllocatorで管理しているため、それを借りる用
		DescriptorAllocator& descriptorAllocator_;
		// SRVの割り当て情報
		DescriptorAllocation srvAllocation_{};
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_{};
		Microsoft::WRL::ComPtr<ID3D12Resource> colorTexture_;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthTexture_;
	};

}