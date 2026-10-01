#pragma once
#include "include/Renderer/Mesh/MeshData.h"
#include <string>
class ObjLoader
{
public:
    MeshData Load(const std::string& filePath);
};  