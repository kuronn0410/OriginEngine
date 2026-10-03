#include "Include/Graphics/Graphics.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include <wrl/client.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_6.h>
#include <dxgi.h>
#include <dxgiformat.h>
#include "Include/Graphics/Color.h"
#include <d3dcommon.h>
/*
DXGI Factory
    ↓ GPUを探す
Adapter
    ↓ このGPUを使う
D3D12 Device
*/

bool Graphics::Initialize(HWND hwnd)
{
	if (!CreateFactory())
	{
		return false;
	}
	if (!CreateAdapter())
	{
		return false;
	}
	if (!CreateDevice())
	{
		return false;
	}
	if (!CreateCommandQueue())
	{
		return false;
	}
	if (!CreateSwapChain(hwnd))
	{
		return false;
	}
	if (!CreateRTVHeap())
	{
		return false;
	}
	if (!CreateRenderTargets())
	{
		return false;
	}
	if(!CreateSRVHeap())
	{ 
		return false;
	}
	if (!CreateCommandAllocator())
	{
		return false;
	}
	if (!CreateCommandList())
	{
		return false;
	}
	if (!CreateFence())
	{
		return false;
	}

	return true;
}

void Graphics::BeginFrame(const Color& clearColor, HWND hwnd)
{
	// ① 現在のBackBuffer(描画する場所)番号取得
	frameIndex_ = swapChain_->GetCurrentBackBufferIndex();
	// ② CommandAllocator Reset
	commandAllocator_->Reset();
	// ③ CommandList Reset
	commandList_->Reset(commandAllocator_.Get(), nullptr);
	// ④ PRESENT → RENDER_TARGET
	D3D12_RESOURCE_BARRIER barrier = {};

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = renderTargets_[frameIndex_].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	// ResourceBarrierをCommandListに記録する
	commandList_->ResourceBarrier(
		1,
		&barrier
	);
	// ⑤ RTV取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle =
		rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	UINT descriptorSize =
		device_->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV
		);
	rtvHandle.ptr += frameIndex_ * descriptorSize;
	// ⑥ 描画先設定
	commandList_->OMSetRenderTargets(
		1,
		&rtvHandle,
		FALSE,
		nullptr
	);
	// ⑥ 描画先設定
	commandList_->OMSetRenderTargets(
		1,
		&rtvHandle,
		FALSE,
		nullptr
	);

	// Viewport設定
	RECT rect{};
	GetClientRect(hwnd, &rect);

	D3D12_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(rect.right - rect.left);
	viewport.Height = static_cast<float>(rect.bottom - rect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	commandList_->RSSetViewports(
		1,
		&viewport
	);

	// ScissorRect設定
	D3D12_RECT scissorRect = {};
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = rect.right - rect.left;
	scissorRect.bottom = rect.bottom - rect.top;

	commandList_->RSSetScissorRects(
		1,
		&scissorRect
	);

	// ⑦ 単色クリア
	float color[4] =
	{
		clearColor.r,
		clearColor.g,
		clearColor.b,
		clearColor.a

	};
	commandList_->ClearRenderTargetView(
		rtvHandle,
		color,
		0,
		nullptr
	);
}

void Graphics::EndFrame()
{
	// ⑧ RENDER_TARGET → PRESENT
	D3D12_RESOURCE_BARRIER returnbarrier = {};
	returnbarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	returnbarrier.Transition.pResource = renderTargets_[frameIndex_].Get();
	returnbarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	returnbarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	returnbarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	// ResourceBarrierをCommandListに記録する
	commandList_->ResourceBarrier(
		1,
		&returnbarrier
	);
	// ⑨ CommandList Close
	HRESULT cmhr = commandList_->Close();

	if (FAILED(cmhr))
	{
		return;
	}
	// ⑩ ExecuteCommandLists
	ID3D12CommandList* commandLists[] =
	{
		commandList_.Get()
	};

	commandQueue_->ExecuteCommandLists(
		1,
		commandLists
	);

	// ⑪ Present
	HRESULT swhr = swapChain_->Present(
		1,
		0
	);

	if (FAILED(swhr))
	{
		return;
	}
	// ⑫ GPU同期
	++fenceValue_;

	HRESULT hr = commandQueue_->Signal(
		fence_.Get(),
		fenceValue_
	);

	if (FAILED(hr))
	{
		return;
	}

	if (fence_->GetCompletedValue() < fenceValue_)//GPUがまだ今回の処理を終えていないなら待つ
	{
		hr = fence_->SetEventOnCompletion(
			fenceValue_,
			fenceEvent_
		);

		if (FAILED(hr))
		{
			return;
		}

		WaitForSingleObject(
			fenceEvent_,
			INFINITE
		);
	}
}

bool Graphics::CreateFactory()
{
	HRESULT hr = CreateDXGIFactory2(
		0,//フラグ
		IID_PPV_ARGS(factory_.GetAddressOf())//ComPtrの中にあるポインタのアドレスを渡す。
	
	);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Graphics::CreateAdapter()
{

	for (UINT i = 0; ; ++i)
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;

		HRESULT hr = factory_->EnumAdapterByGpuPreference(
			i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(adapter.GetAddressOf())
		);

		if (hr == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}

		if (FAILED(hr))
		{
			return false;
		}


		DXGI_ADAPTER_DESC1 desc{};
		adapter->GetDesc1(&desc);

		// ソフトウェアGPUは除外
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		// DirectX12が使用可能か確認
		if (SUCCEEDED(D3D12CreateDevice(
			adapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			__uuidof(ID3D12Device),
			nullptr)))
		{
			adapter_ = adapter;
			return true;
		}
		
	}
	return false;
}

bool Graphics::CreateDevice()
{

	HRESULT hr = D3D12CreateDevice(
		adapter_.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(device_.GetAddressOf())//書き込んでもらう
		);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}	

bool Graphics::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	//queueの
	desc.Priority
		= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type
		= D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Flags
		= D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask
		= 0;
	HRESULT hr = device_->CreateCommandQueue(
		&desc,
		IID_PPV_ARGS(commandQueue_.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}
	return true;
}



bool Graphics::CreateSwapChain(HWND hwnd)
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

	HRESULT hr = factory_->CreateSwapChainForHwnd(
		commandQueue_.Get(),
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


bool Graphics::CreateRTVHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.NumDescriptors = kFrameCount;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;


	HRESULT hr = device_->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(rtvHeap_.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}


bool Graphics::CreateRenderTargets()
{
	// ① RTV Heap の先頭アドレスを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		rtvHeap_->GetCPUDescriptorHandleForHeapStart();

	// ② RTV 1個分のサイズを取得
	UINT descriptorSize =
		device_->GetDescriptorHandleIncrementSize(
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

		device_->CreateRenderTargetView(
			renderTargets_[i].Get(), 
			&rtvDesc, 
			handle
		);
		// ⑤ handle を次の RTV の位置へ進める
		handle.ptr += descriptorSize;
	}

	return true;
}

bool Graphics::CreateSRVHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = kMaxSrvCount;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;


	HRESULT hr = device_->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(srvHeap_.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	srvDescriptorSize_ =
		device_->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);

	return true;
}
bool Graphics::CreateCommandAllocator()
{
	HRESULT hr = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(commandAllocator_.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Graphics::CreateCommandList()
{
	HRESULT hr = device_->CreateCommandList(
		/* ① NodeMask */
		0,
		/* ② CommandListの種類 */
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		/* ③ 使用するCommandAllocator */
		commandAllocator_.Get(),
		/* ④ 初期PipelineState */
		nullptr,
		IID_PPV_ARGS(commandList_.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	// ⑤ 初期状態ではOpenなので、必要ならCloseする
	commandList_->Close();
	return true;
}

bool Graphics::CreateFence() 
{
	HRESULT hr = device_->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(fence_.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

