#pragma once
#include <filesystem>
#include <string>

namespace Origin::Editor
{
    struct GameProject
    {
        std::string name;
        std::filesystem::path rootPath;
        std::filesystem::path projectFilePath;
    };
}