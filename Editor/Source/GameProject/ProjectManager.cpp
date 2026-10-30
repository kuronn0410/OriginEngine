#include "Include/GameProject/ProjectManager.h"
#include "Include/GameProject/GameProjectLoader.h"
#include "Include/GameProject/GameProject.h"

namespace Origin::Editor
{
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
}