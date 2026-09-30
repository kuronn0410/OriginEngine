#pragma once
#include "include/Renderer/Mesh/Vertex.h"
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