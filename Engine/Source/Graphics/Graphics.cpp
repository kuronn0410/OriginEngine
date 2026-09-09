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

bool Graphics::Initialize()
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

	return false;
}	