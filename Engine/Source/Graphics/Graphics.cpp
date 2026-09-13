#include "Include/Graphics/Graphics.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
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

	return true;
}

bool Graphics::CreateFactory()
{
	HRESULT hr = CreateDXGIFactory2(
		0,//フラグ
		IID_PPV_ARGS(factory_.GetAddressOf())//ComPtrの中にあるポインターのアドレスを渡す。
	
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

		/*---------ここから----------*/
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
	/*
	adapter_ が保持しているGPUを使って
	↓
	最低 Feature Level 11_0 を要求し
	↓
	ID3D12Device を生成して
	↓
	device_ に書き込んでもらう
	↓
	HRESULTで成功/失敗を確認する
	*/

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
	desc.BufferCount = 2;
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
	desc.NumDescriptors = 2;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	// ① Heapの種類
	// ② Descriptorを何個置くか
	// ③ Shaderから見える必要があるか
	// ④ NodeMask

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