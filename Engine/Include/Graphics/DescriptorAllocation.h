#pragma once
#include <d3d12.h>
#include <cstdint>
struct DescriptorAllocation
{
    uint32_t index = UINT32_MAX;

    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle{};
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

    bool IsValid() const
    {
        return index != UINT32_MAX;
    }
};