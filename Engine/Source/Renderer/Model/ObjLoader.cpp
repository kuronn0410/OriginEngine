#include "include/Renderer/Model/ObjLoader.h"
#include "include/Renderer/Mesh/MeshData.h"
#include <string>
#include <fstream>
#include <sstream>

MeshData ObjLoader::Load(const std::string& filePath)
{
    MeshData meshData;

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
            meshData.vertices.push_back({x, y, z}); // 位置情報を解析
        }
        // fならindicesへ
        if (line.substr(0, 2) == "f ")
        {
            std::istringstream stream(line);
            
            std::string type;
            std::string vertex1;
            std::string vertex2;
            std::string vertex3;
            stream >> type >> vertex1 >> vertex2 >> vertex3;

            size_t slashPos = vertex1.find('/');
            std::string indexText = vertex1.substr(0, slashPos);
            int v1 = std::stoi(indexText) - 1;

            slashPos = vertex2.find('/');
            indexText = vertex2.substr(0, slashPos);
            int v2 = std::stoi(indexText) - 1;

            slashPos = vertex3.find('/');
            indexText = vertex3.substr(0, slashPos);
            int v3 = std::stoi(indexText) - 1;

            meshData.indices.push_back(v1);
            meshData.indices.push_back(v2);
            meshData.indices.push_back(v3);
        }

    }

    

    return meshData;
}