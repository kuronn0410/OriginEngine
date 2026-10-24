#pragma once
#include "include/Math/Vector3.h"
namespace Origin
{
    struct CameraTransform
    {
        Vector3 position{ 0.0f, 0.0f, 0.0f };
        Vector3 rotation{ 0.0f, 0.0f, 0.0f };
    };
}