#pragma once
#include <filesystem>
#include <string>

struct GameProject
{
    std::string name;
    std::filesystem::path rootPath;
    std::filesystem::path projectFilePath;
};