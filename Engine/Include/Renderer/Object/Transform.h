#pragma once
#include "include/Math/Vector3.h"
/// <summary>
/// オブジェクトのTransform情報
/// </summary>
struct Transform
{
    Vector3 position{ 0.0f, 0.0f, 0.0f };
    Vector3 rotation{ 0.0f, 0.0f, 0.0f };
    Vector3 scale{ 1.0f, 1.0f, 1.0f };
};