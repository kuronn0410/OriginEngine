#pragma once
#include <Windows.h>
#include <dxgiformat.h>
#include <vector>
#include <cstddef>
namespace Origin
{
    struct TextureData
    {
        UINT width;
        UINT height;
        DXGI_FORMAT format;

        std::vector<std::byte> pixels;
    };
}