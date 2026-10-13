#pragma once
#include <cstdint>
struct WindowHandle
{
    uint32_t id = UINT32_MAX;

    bool IsValid() const
    {
        return id != UINT32_MAX;
    }
};