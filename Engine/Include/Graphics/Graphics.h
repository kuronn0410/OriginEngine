#pragma once
#include <d3d12.h>//D3D12CreateDevice などDirectX12本体
#include <dxgi1_6.h>//IDXGIFactory6 などDXGI本体
#include <wrl.h>//Microsoft::WRL::ComPtr などのスマートポインタ



class Graphics
{
public:
	bool Initialize();
	bool Update();
	bool Finalize();
private:
	bool CreateFactory();
	bool CreateAdapter();
	bool CreateDevice();

	Microsoft::WRL::ComPtr<IDXGIFactory6> factory_;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
};