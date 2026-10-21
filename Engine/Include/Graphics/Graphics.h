#pragma once
#include <d3d12.h>//D3D12CreateDevice などDirectX12本体
#include <dxgi1_6.h>//IDXGIFactory6 などDXGI本体
#include <wrl.h>//Microsoft::WRL::ComPtr などのスマートポインタ
#include <wrl/client.h>
#include <dxgi.h>
#include <dxgi1_5.h>
#include "include/Graphics/Color.h"
#include "include/Graphics/SwapChainContext.h"
#include <Windows.h>
#include "include/Graphics/DescriptorAllocator.h"
#include <optional>


class Graphics
{
public:
	bool Initialize(HWND hwnd);
	void Render(const Color& clearColor);// 1フレームの描画
	bool BeginInitializationCommands();
	bool EndInitializationCommands();
	void BeginFrame(const Color& clearColor, HWND hwnd, SwapChainContext& swapChainContext);
	void EndFrame(SwapChainContext& swapChainContext);
	bool Update();
	bool Finalize();
	/*----取得関数------*/
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	ID3D12Device* GetDevice() const { return device_.Get(); }
	IDXGIFactory6* GetFactory() const { return factory_.Get(); }
	ID3D12CommandQueue* GetCommandQueue() const { return commandQueue_.Get(); }
	ID3D12DescriptorHeap* GetSRVHeap() const{return srvHeap_.Get();}
	UINT GetSRVDescriptorSize() const{return srvDescriptorSize_;}
	UINT GetMaxSRVCount() const { return kMaxSrvCount; }
	DescriptorAllocator* GetDescriptorAllocator() { return &descriptorAllocator_.value(); }
private:
	bool CreateFactory();//オブジェクトの生成の入り口
	bool CreateAdapter();//使えるGPUを探す
	bool CreateDevice();//GPUを使って、DirectX12の機能を使えるようにする
	bool CreateCommandQueue(); //CPU側で作ったGPU命令を、GPUへ送るためのQueueの形式を決める
	bool CreateSRVHeap();//SRV(Shader Resource View「このTextureをShaderで使う」という指定)を置くための場所を作れるようにする
	bool CreateDescriptorAllocator();	
	bool CreateCommandAllocator();
	bool CreateCommandList();
	bool CreateFence();
	

	/*---------初期化-----------*/
	Microsoft::WRL::ComPtr<IDXGIFactory6> factory_;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	static constexpr UINT kFrameCount = 2;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;//SRV用のHeap
	static constexpr UINT kMaxSrvCount = 256;
	std::optional<DescriptorAllocator> descriptorAllocator_;//再構築はしないのでoptionalで管理
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	/*---------毎フレームの描画処理-----------*/
	 // 描画補助
	void WaitForGPU();
	UINT frameIndex_ = 0;
	UINT64 fenceValue_ = 0;
	HANDLE fenceEvent_ = nullptr;
	UINT srvDescriptorSize_ = 0;
};