#pragma once
#include <filesystem>
#include "GameProject.h"
#include "Include/GameProject/ProjectSaveData.h"
#include <vector>

namespace Origin::Editor
{
	class ProjectStorage
	{
	public:
		bool ProjectSave(std::vector<GameProject> projectPaths, std::filesystem::path projectPath);
		ProjectSaveData ProjectLoad();
	private:
		bool FindStandardFile();
		std::filesystem::path path_;
	};
} // namespace Origin::Editor