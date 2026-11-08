#pragma once
#include "Include/GameProject/GameProject.h"
#include "Include/GameProject/GameProjectLoader.h"
#include"Include/GameProject/ProjectStorage.h"
#include <filesystem> 
#include <optional>
namespace Origin::Editor
{

	class ProjectManager
	{
	public:
		//初期化
		bool Initialize();
		//既存のプロジェクトの登録
		bool InitialRegistration(const std::filesystem::path& directory);
		//プロジェクトの読み込み
		bool LoadProject(const std::filesystem::path& directory);
		void CloseProject() { currentProject_ = std::nullopt; } // Reset current project
		/*===取得===*/
		const GameProject* GetCurrentProject() const { return currentProject_ ? &(*currentProject_) : nullptr;}
		const std::vector<GameProject>& GetLoadedProjects() const { return loadedProjects_; }
		//void  SetCurrentProjectId(size_t index);
	private:
		bool SaveProjectToFile();
		GameProjectLoader projectLoader_;
		ProjectStorage projectStorage_;
		std::optional<GameProject> currentProject_;
		//size_t currentProjectIndex_ = 0;
		std::vector<GameProject> loadedProjects_;

	};
}