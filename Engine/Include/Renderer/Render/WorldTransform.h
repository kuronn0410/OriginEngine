#pragma once
#include "include/Math/Matrix4x4.h"

struct WorldTransform
{
    Matrix4x4 world;
    Matrix4x4 view;
    Matrix4x4 projection;
};