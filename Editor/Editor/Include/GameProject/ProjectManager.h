#pragma once
#include "Include/GameProject/GameProject.h"
#include "Include/GameProject/GameProjectLoader.h"
#include <filesystem> 
#include <optional>
namespace Origin::Editor
{

	class ProjectManager
	{
	public:
		bool LoadProject(const std::filesystem::path& directory);
		const GameProject* GetCurrentProject() const { return &currentProject_.value(); }
		void CloseProject() { currentProject_ = std::nullopt; } // Reset current project

	private:
		GameProjectLoader projectLoader_;
		std::optional<GameProject> currentProject_;
	};
}