#include "include/Graphics/SwapChainContext.h"
#include <dxgi1_6.h>//IDXGIFactory6 などDXGI本体
#include <dxgi1_2.h>//IDXGISwapChain4 などDXGI本体
#include <wrl/client.h>//Microsoft::WRL::ComPtr などのスマートポインタ
#include <dxgi.h>//IDXGISwapChain1 などDXGI本体
#include <dxgiformat.h>//DXGI_FORMAT_R8G8B8A8_UNORM などDXGI本体
#include <d3d12.h>//ID3D12Device などD3D12本体
#include <Windows.h>
/*CreateSwapChain*/

namespace Origin
{
	bool SwapChainContext::Initialize(
		HWND hwnd,
		IDXGIFactory6& factory,
		ID3D12CommandQueue& commandQueue,
		ID3D12Device& device

	)
	{
		if (!CreateSwapChain(hwnd, factory, commandQueue))
		{
			return false;
		}
		if (!CreateRTVHeap(device))
		{
			return false;
		}
		if (!CreateRenderTargets(device))
		{
			return false;
		}
		if (!CreateDSVHeap(device))
		{
			return false;
		}
		if (!CreateDepthBuffer(device, hwnd))
		{
			return false;
		}
		return true;
	}

	bool SwapChainContext::CreateSwapChain(
		HWND hwnd,
		IDXGIFactory6& factory,
		ID3D12CommandQueue& commandQueue
	)
	{
		DXGI_SWAP_CHAIN_DESC1 desc = {};

		RECT rect{};
		GetClientRect(hwnd, &rect);//ウィンドウのサイズを取得する

		desc.Height = rect.bottom - rect.top;
		desc.Width = rect.right - rect.left;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferCount = kFrameCount;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//裏表を入れ替える
		desc.Flags = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SampleDesc.Count = 1;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;

		HRESULT hr = factory.CreateSwapChainForHwnd(
			&commandQueue,
			hwnd,
			&desc,
			nullptr,
			nullptr,
			swapChain.GetAddressOf()
		);

		if (FAILED(hr))
		{
			return false;
		}

		// ⑥ IDXGISwapChain1 → IDXGISwapChain4 に変換して保持
		HRESULT asHr = swapChain.As(&swapChain_);
		if (FAILED(asHr))
		{
			return false;
		}

		return true;
	}






	bool SwapChainContext::CreateRTVHeap(ID3D12Device& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};

		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors = kFrameCount;
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





	bool SwapChainContext::CreateRenderTargets(ID3D12Device& device)
	{
		// ① RTV Heap の先頭アドレスを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle =
			rtvHeap_->GetCPUDescriptorHandleForHeapStart();

		// ② RTV 1個分のサイズを取得
		UINT descriptorSize =
			device.GetDescriptorHandleIncrementSize(
				D3D12_DESCRIPTOR_HEAP_TYPE_RTV
			);

		for (UINT i = 0; i < kFrameCount; ++i)
		{
			// ③ SwapChain から i 番目の BackBuffer を取得
			HRESULT hr = swapChain_->GetBuffer(
				i,
				IID_PPV_ARGS(renderTargets_[i].GetAddressOf())
			);

			if (FAILED(hr))
			{
				return false;
			}

			// ④ その BackBuffer 用の RTV を作る
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

			device.CreateRenderTargetView(
				renderTargets_[i].Get(),
				&rtvDesc,
				handle
			);
			// ⑤ handle を次の RTV の位置へ進める
			handle.ptr += descriptorSize;
		}

		return true;
	}





	bool SwapChainContext::CreateDSVHeap(ID3D12Device& device)
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

	bool SwapChainContext::CreateDepthBuffer(ID3D12Device& device,HWND hwnd)
	{

		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

		RECT rect{};
		GetClientRect(hwnd, &rect);
		UINT width = rect.right - rect.left;
		UINT height = rect.bottom - rect.top;

		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Width = width;
		resourceDesc.Height = height;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE clearValue{};
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		HRESULT hr = device.CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(depthBuffer_.GetAddressOf())
		);

		if (FAILED(hr))
		{
			return false;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
			dsvHeap_->GetCPUDescriptorHandleForHeapStart();

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		device.CreateDepthStencilView(
			depthBuffer_.Get(),
			&dsvDesc,
			dsvHandle
		);

		return true;
	}


	/*-----------------取得-----------------*/
	DXGI_FORMAT SwapChainContext::GetRTVFormat() const
	{
		DXGI_SWAP_CHAIN_DESC1 desc{};

		if (FAILED(swapChain_->GetDesc1(&desc)))
		{
			return DXGI_FORMAT_UNKNOWN;
		}

		return desc.Format;
	}

	UINT SwapChainContext::GetBufferCount() const
	{
		DXGI_SWAP_CHAIN_DESC1 desc{};

		if (FAILED(swapChain_->GetDesc1(&desc)))
		{
			return 0;
		}

		return desc.BufferCount;
	}
}