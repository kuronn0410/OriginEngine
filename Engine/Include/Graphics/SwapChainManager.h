#pragma once
#include <memory>
#include <unordered_map>
#include <Windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include "Include/Graphics/SwapChainContext.h"
#include "Include/Window/WindowHandle.h"
namespace Origin
{
    class SwapChainManager
    {
    public:
        SwapChainManager(
            IDXGIFactory6& factory,
            ID3D12Device& device,
            ID3D12CommandQueue& commandQueue
        ) :
            factory_(factory),
            device_(device),
            commandQueue_(commandQueue)
        {
        }


        bool Create(
            WindowHandle handle,
            HWND hwnd
        );

        SwapChainContext* GetContext(WindowHandle handle);
        DXGI_FORMAT GetRTVFormat(WindowHandle handle);
        UINT GetBufferCount(WindowHandle handle);

        bool Destroy(WindowHandle handle);

        bool DestroyAll();
    private:
        IDXGIFactory6& factory_;
        ID3D12Device& device_;
        ID3D12CommandQueue& commandQueue_;

        std::unordered_map<
            uint32_t,
            std::unique_ptr<SwapChainContext>
        > contexts_;

    };
}