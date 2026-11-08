#include "Include/GameProject/ProjectManager.h"
#include "Include/GameProject/GameProjectLoader.h"
#include "Include/GameProject/GameProject.h"
#include "Include/GameModuleLoader/GameModuleLoader.h"
#include "Include/GameProject/ProjectSaveData.h"

namespace Origin::Editor
{
	bool ProjectManager::Initialize()
	{
		ProjectSaveData saveData = projectStorage_.ProjectLoad();
		if (saveData.currentProjectPath.has_value())
		{
			for (const auto& path : saveData.projectPaths)
			{
				auto project = projectLoader_.LoadProject(path);

				if (!project.has_value())
				{
					return false;
				}

				loadedProjects_.emplace_back(std::move(project.value()));
			}
		}
		// 未選択での起動を許可する
		currentProject_.reset();

		if (saveData.currentProjectPath.has_value() &&
			!saveData.currentProjectPath->empty())
		{
			currentProject_ =
				projectLoader_.LoadProject(*saveData.currentProjectPath);

			if (!currentProject_.has_value())
			{
				return false;
			}
		}
		return true;
	}

	bool ProjectManager::InitialRegistration(const std::filesystem::path& directory)
	{
		LoadProject(directory);
		loadedProjects_.push_back(*currentProject_);
		if (!projectStorage_.ProjectSave(loadedProjects_, currentProject_.value().rootPath))
		{
			return false; // Failed to save project
		}
		return true;
	}

	bool ProjectManager::LoadProject(const std::filesystem::path& directory)
	{

		auto loaded = std::move(projectLoader_.LoadProject(directory));

		if (!loaded.has_value())
		{
			return false; // Failed to load project
		}
		currentProject_ = std::move(loaded);
		return true;
	}

	//void  ProjectManager::SetCurrentProjectId(size_t index)
	//{
	//	if (index < loadedProjects_.size())
	//	{
	//		currentProjectIndex_ = index;
	//		currentProject_ = loadedProjects_[index];
	//	}
	//	else
	//	{
	//		currentProject_ = std::nullopt; // Reset current project if index is out of bounds
	//	}
	//}
}