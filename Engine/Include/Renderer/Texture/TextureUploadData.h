#pragma once
#include <d3d12.h>
#include <wrl/client.h>
struct TextureUploadData
{
    Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};
};