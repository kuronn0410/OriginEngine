#pragma once
#include "include/Renderer/Mesh/MeshData.h"
#include "include/Renderer/Model/ObjVertexIndex.h"
#include <string>
namespace Origin
{
	class ObjLoader
	{
	public:
		MeshData Load(const std::string& filePath);
	private:
		ObjVertexIndex ParseVertex(const std::string& vertexString);
	};
}