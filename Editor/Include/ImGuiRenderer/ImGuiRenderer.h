#pragma once
#include <d3d12.h>
#include <Windows.h>
#include <dxgiformat.h>
#include "Include/Graphics/DescriptorAllocator.h"

namespace Origin
{
    class ImGuiRenderer
    {
    public:
        ImGuiRenderer(
            DescriptorAllocator& allocator,
            ID3D12DescriptorHeap& srvHeap
        )
            : descriptorAllocator_(allocator),
            srvHeap_(srvHeap)
        {
        }
        bool Initialize(
            //Graphics側
            ID3D12Device* device,
            ID3D12CommandQueue* commandQueue,
            //スワップチェイン側
            int numFramesInFlight,
            DXGI_FORMAT rtvFormat,
            //ウィンドウ側
            HWND hwnd
        );

        void BeginFrame();

        void Render(
            ID3D12GraphicsCommandList& commandList

        );

        void Shutdown();
    private:
        DescriptorAllocator& descriptorAllocator_;
        ID3D12DescriptorHeap& srvHeap_;
    };
}