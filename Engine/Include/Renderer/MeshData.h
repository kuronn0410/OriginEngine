#pragma once
#include "include/Math/Vector3.h"
#include "include/Renderer/Vertex.h"
#include <vector>
#include <cstdint>
/// <summary>
/// Meshの初期値をまとめた構造体
/// </summary>
struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};