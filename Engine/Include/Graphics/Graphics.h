#pragma once
#include <d3d12.h>//D3D12CreateDevice などDirectX12本体
#include <dxgi1_6.h>//IDXGIFactory6 などDXGI本体
#include <wrl.h>//Microsoft::WRL::ComPtr などのスマートポインタ
#include <wrl/client.h>
#include <dxgi.h>
#include <dxgi1_5.h>
#include "include/Graphics/Color.h"
#include <Windows.h>



class Graphics
{
public:
	bool Initialize(HWND hwnd);
	void Render(const Color& clearColor);// 1フレームの描画
	void BeginFrame(const Color& clearColor, HWND hwnd);
	void EndFrame();
	bool Update();
	bool Finalize();
	/*----取得関数------*/
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	ID3D12Device* GetDevice() const { return device_.Get(); }
private:
	bool CreateFactory();//オブジェクトの生成の入り口
	bool CreateAdapter();//使えるGPUを探す
	bool CreateDevice();//GPUを使って、DirectX12の機能を使えるようにする
	bool CreateCommandQueue(); //CPU側で作ったGPU命令を、GPUへ送るためのQueueの形式を決める
	bool CreateSwapChain(HWND hwnd); // 描画する画像（BackBuffer）と、画面に表示する画像を切り替える仕組み
	bool CreateRTVHeap();//RTV(Render Target View「このBackBufferを描画先として使う」という指定)を置くための場所を作れるようにする
	bool CreateRenderTargets();//SwapChain が持っている各 BackBuffer を取得して、RTV Heap の各スロットに RTV を作る
	bool CreateCommandAllocator();
	bool CreateCommandList();
	bool CreateFence();

	/*---------初期化-----------*/
	Microsoft::WRL::ComPtr<IDXGIFactory6> factory_;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	static constexpr UINT kFrameCount = 2;
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets_[kFrameCount];
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	

	/*---------毎フレームの描画処理-----------*/
	 // 描画補助
	void WaitForGPU();
	UINT frameIndex_ = 0;
	UINT64 fenceValue_ = 0;
	HANDLE fenceEvent_ = nullptr;
};