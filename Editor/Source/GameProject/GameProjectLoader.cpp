#include "Include/GameProject/GameProjectLoader.h"
#include <filesystem>
#include <iostream>
#include <optional>
#include "Include/GameProject/GameProject.h"

std::optional<GameProject> GameProjectLoader::LoadProject(const std::filesystem::path& _directory)
{

	std::optional<GameProject> project= std::nullopt;
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(_directory))
	{
		if (entry.is_regular_file())
		{
			if (entry.path().extension() == ".vcxproj")
			{
				if (project.has_value())
				{
					return std::nullopt; // Multiple project files found, return nullopt
				}
				project.emplace(); //GameProjectのインスタンスを作成
				project.value().name = entry.path().stem().string();
				project.value().rootPath = _directory;
				project.value().projectFilePath = entry.path();
			}
		}
	}
	if(!project.has_value())
	{
		return std::nullopt; // No project file found, return nullopt
	}
	return project;
}