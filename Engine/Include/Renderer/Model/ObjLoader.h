#pragma once
#include "include/Renderer/Mesh/MeshData.h"
#include "include/Renderer/Model/ObjVertexIndex.h"
#include <string>
class ObjLoader
{
public:
    MeshData Load(const std::string& filePath);
private:
	ObjVertexIndex ParseVertex(const std::string& vertexString);
};  