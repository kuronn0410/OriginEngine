#include "include/Renderer/Model/ObjLoader.h"
#include "include/Renderer/Mesh/MeshData.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include "include/Math/Vector3.h"
#include "include/Math/Vector2.h"
#include "include/Renderer/Model/ObjVertexIndex.h"

MeshData ObjLoader::Load(const std::string& filePath)
{
    MeshData meshData;
    std::vector<Vector3> positions;
    std::vector<Vector2> texcoords;
    std::vector<Vector3> normals;
    // ファイルを開く
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return meshData;
    }

    // 1行ずつ読む
    std::string line;

    while (std::getline(file, line))
    {
        // 1行ずつ解析
        // vならverticesへ
        if (line.substr(0, 2) == "v ")
        {
            std::istringstream stream(line);

            std::string type;
            float x;
            float y;
            float z;

            stream >> type >> x >> y >> z;
            positions.push_back({x, y, z}); // 位置情報を解析
        }

        //vn
        if (line.substr(0, 3) == "vn ")
        {
            std::istringstream stream(line);

            std::string type;
            float x;
            float y;
            float z;

            stream >> type >> x >> y >> z;
            normals.push_back({ x, y, z }); // 位置情報を解析
        }
        //vt
        if (line.substr(0, 3) == "vt ")
        {
            std::istringstream stream(line);

            std::string type;

			float u;
			float v;
			stream >> type >> u >> v;
            texcoords.push_back({ u, v }); // 位置情報を解析
        }
        // fならindicesへ
        

    }

    file.clear();
    file.seekg(0, std::ios::beg);

    while (std::getline(file, line))
    {
        if (line.substr(0, 2) == "f ")
        {
            std::istringstream stream(line);
            uint32_t baseIndex =
                static_cast<uint32_t>(meshData.vertices.size());
            std::string type;
            std::string vertex1;
            std::string vertex2;
            std::string vertex3;
            
            Vector2 uv1{ 0.0f, 0.0f };
            Vector2 uv2{ 0.0f, 0.0f };
            Vector2 uv3{ 0.0f, 0.0f };

            stream >> type >> vertex1 >> vertex2 >> vertex3;

			ObjVertexIndex index1 = ParseVertex(vertex1);
            ObjVertexIndex index2 = ParseVertex(vertex2);
            ObjVertexIndex index3 = ParseVertex(vertex3);

            if (index1.uv != -1)
            {
                uv1 = texcoords[index1.uv];
            }
            meshData.vertices.push_back({
                positions[index1.position],
                uv1,
                normals[index1.normal]
                });

            if (index2.uv != -1)
            {
                uv2 = texcoords[index2.uv];
            }
            meshData.vertices.push_back({
                positions[index2.position],
                uv2,
                normals[index2.normal]
                });
			if(index3.uv != -1)
			{
				uv3 = texcoords[index3.uv];
			}

            meshData.vertices.push_back({
                positions[index3.position],
                uv3,
                normals[index3.normal]
                });

            meshData.indices.push_back(baseIndex);
            meshData.indices.push_back(baseIndex + 1);
            meshData.indices.push_back(baseIndex + 2);
        }
    }
    return meshData;
}

/// <summary>
/// 頂点情報を解析する
/// </summary>
ObjVertexIndex ObjLoader::ParseVertex(const std::string& vertexString)
{
    ObjVertexIndex indexes;
    
    std::string number;
	int slashCount = 0;
	int currentIndex = 0;
    for (size_t i = 0; i < vertexString.size(); ++i)
    {
        char c = vertexString[i];
        if (c == '/')
        {
            if (!number.empty())
            {
                int index = std::stoi(number) - 1;
                switch (currentIndex)
                {
                case 0:
                    indexes.position = index;
                    break;
				case 1:
					indexes.uv = index;
                    break;
				case 2:
					indexes.normal = index;
                    break;
                }
                number.clear();
               
            }
            currentIndex++;
			
        }
        else
        {
            number += c;
          
        }
    }

    if (!number.empty())
    {
        int index = std::stoi(number) - 1;
        switch (currentIndex)
        {
        case 0:
            indexes.position = index;
            break;
        case 1:
            indexes.uv = index;
            break;
        case 2:
            indexes.normal = index;
            break;
        }
    }
    return indexes;
}