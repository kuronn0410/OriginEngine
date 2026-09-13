#pragma once
#include <d3d12.h>//D3D12CreateDevice などDirectX12本体
#include <dxgi1_6.h>//IDXGIFactory6 などDXGI本体
#include <wrl.h>//Microsoft::WRL::ComPtr などのスマートポインタ



class Graphics
{
public:
	bool Initialize(HWND hwnd);
	bool Update();
	bool Finalize();
private:
	bool CreateFactory();//オブジェクトの生成の入り口
	bool CreateAdapter();//使えるGPUを探す
	bool CreateDevice();//GPUを使って、DirectX12の機能を使えるようにする
	bool CreateCommandQueue(); //CPU側で作ったGPU命令を、GPUへ送るためのQueueの形式を決める
	bool CreateSwapChain(HWND hwnd); // 描画する画像（BackBuffer）と、画面に表示する画像を切り替える仕組み
	bool CreateRTVHeap();//RTV(Render Target View「このBackBufferを描画先として使う」という指定)を置くための場所を作る
	bool CreateRenderTargets();
	bool CreateCommandAllocator();
	bool CreateCommandList();
	bool CreateFence();

	Microsoft::WRL::ComPtr<IDXGIFactory6> factory_;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
};