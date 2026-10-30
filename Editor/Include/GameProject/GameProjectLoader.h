#pragma once
#include "Include/GameProject/GameProject.h"
#include <filesystem>
#include <optional>
namespace Origin::Editor
{
	class GameProjectLoader
	{
	public:
		std::optional<GameProject> LoadProject(const std::filesystem::path& directory);

	};
}