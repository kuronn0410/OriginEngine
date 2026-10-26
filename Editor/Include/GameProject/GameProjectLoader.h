#pragma once
#include "Include/GameProject/GameProject.h"
#include <filesystem>
#include <optional>
class GameProjectLoader
{
public:
	std::optional<GameProject> LoadProject(const std::filesystem::path& directory);

};