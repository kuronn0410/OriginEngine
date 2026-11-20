#pragma once
#include <cstdint>
namespace Origin
{
    struct OffscreenHandle
    {
        uint32_t id = UINT32_MAX;

        bool IsValid() const
        {
            return id != UINT32_MAX;
        }
    };
}