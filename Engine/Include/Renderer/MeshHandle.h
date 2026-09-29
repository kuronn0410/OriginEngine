#pragma once
#include <cstdint>
/// <summary>
/// IDを保持するための構造体
/// </summary>
struct MeshHandle
{
    uint32_t id = UINT32_MAX;

    bool IsValid() const
    {
        return id != UINT32_MAX;
    }
};