#pragma once
#include "include/Math/Vector3.h"
#include "include/Math/Vector2.h"
/// <summary>
/// 一つの頂点の情報をまとめた構造体
/// </summary>
struct Vertex
{
    Vector3 position;
    Vector2 uv;
    Vector3 normal;
};