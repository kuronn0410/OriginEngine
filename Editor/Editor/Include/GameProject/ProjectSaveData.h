#pragma once
#include <filesystem>
#include <optional>
#include <vector>
    
struct ProjectSaveData
{
    std::optional<std::filesystem::path> currentProjectPath;
    std::vector<std::filesystem::path> projectPaths;
};