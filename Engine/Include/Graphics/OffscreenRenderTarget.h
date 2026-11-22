#pragma once
#include "Include/Graphics/DescriptorAllocator.h"
#include <Windows.h>
#include <wrl/client.h>
#include <d3d12.h>
namespace Origin
{
	class OffscreenRenderTarget
	{
	public:
		//OffscreenRenderTarget(DescriptorAllocator& descriptorAllocator) : 
		//	descriptorAllocator_(descriptorAllocator) {}
		bool Initialize(
			ID3D12Device& device,
			UINT width,
			UINT height,
			const DescriptorAllocation& allocation
		);
		const DescriptorAllocation GetSRVAllocation() const { return srvAllocation_; }
		ID3D12Resource* GetRenderTarget() const { return colorTexture_.Get(); }
		ID3D12Resource* GetDepthBuffer() const { return depthTexture_.Get(); }
		ID3D12DescriptorHeap* GetRTVHeap() const { return rtvHeap_.Get(); }
		ID3D12DescriptorHeap* GetDSVHeap() const { return dsvHeap_.Get(); }
		UINT GetWidth() const { return width_; }
		UINT GetHeight() const { return height_;}
		void SetCurrentState(D3D12_RESOURCE_STATES state) { currentState_ = state; }
		D3D12_RESOURCE_STATES GetCurrentState() const { return currentState_; }

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
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_{};
		DescriptorAllocation srvAllocation_{};
		Microsoft::WRL::ComPtr<ID3D12Resource> colorTexture_;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthTexture_;
		D3D12_RESOURCE_STATES currentState_ =
			D3D12_RESOURCE_STATE_RENDER_TARGET;
		UINT width_ = 0;
		UINT height_ = 0;
	};

}