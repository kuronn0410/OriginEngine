#pragma once
#include <cstdint>
namespace Origin
{
    struct WindowHandle
    {
        uint32_t id = UINT32_MAX;

        bool IsValid() const
        {
            return id != UINT32_MAX;
        }
    };
}