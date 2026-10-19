#pragma once
#include <d3d12.h>
#include <Windows.h>

class ImGuiRenderer
{
public:
    bool Initialize(
        ID3D12Device* device,
        HWND hwnd
        // Descriptor管理機能など
    );

    void BeginFrame();

    void Render(
        ID3D12GraphicsCommandList* commandList
    );

    void Shutdown();
};