#pragma once
#include <Windows.h>
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <dxgi1_5.h>
#include <d3d12.h>


namespace Origin
{
	class SwapChainContext
	{
	public:
		bool Initialize(
			HWND hwnd,
			IDXGIFactory6& factory,
			ID3D12CommandQueue& commandQueue,
			ID3D12Device& device
		);

		IDXGISwapChain4* GetSwapChain() const { return swapChain_.Get(); }
		ID3D12DescriptorHeap* GetRTVHeap() const { return rtvHeap_.Get(); }
		ID3D12DescriptorHeap* GetDSVHeap() const { return dsvHeap_.Get(); }
		ID3D12Resource* GetRenderTarget(UINT index) const { return renderTargets_[index].Get(); }
		ID3D12Resource* GetDepthBuffer() const { return depthBuffer_.Get(); }
		DXGI_FORMAT GetRTVFormat() const;
		UINT GetBufferCount() const;
	private:
		bool CreateSwapChain(HWND hwnd, IDXGIFactory6& factory, ID3D12CommandQueue& commandQueue); // 描画する画像（BackBuffer）と、画面に表示する画像を切り替える仕組み
		bool CreateRTVHeap(ID3D12Device& device);//RTV(Render Target View「このBackBufferを描画先として使う」という指定)を置くための場所を作れるようにする
		bool CreateRenderTargets(ID3D12Device& device);//SwapChain が持っている各 BackBuffer を取得して、RTV Heap の各スロットに RTV を作る
		bool CreateDSVHeap(ID3D12Device& device);
		bool CreateDepthBuffer(ID3D12Device& device, HWND hwnd);
		static constexpr UINT kFrameCount = 2;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;

		Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets_[kFrameCount];
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;


	};
}