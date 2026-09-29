#pragma once
#include "include/Math/Vector3.h"
#include "include/Renderer/Vertex.h"
#include <vector>
/// <summary>
/// Meshの初期値をまとめた構造体
/// </summary>
struct MeshData
{
    //Vector3 position;
    std::vector<Vertex> vertices;
};