#include "Include/GameProject/ProjectStorage.h"
#include <ShlObj_core.h>
#include <Windows.h>
#include <KnownFolders.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Include/GameProject/GameProject.h"

using json = nlohmann::json;
namespace Origin::Editor
{
	bool ProjectStorage::ProjectSave(std::vector<GameProject> projectPaths, std::filesystem::path currentProjectPath)
	{
		if (!FindStandardFile())
		{
			return false;
		}
		//ない場合はディレクトリを作成する
		namespace fs = std::filesystem;
		fs::create_directories(path_ / "OriginEngine/Engine");
		std::ofstream ofs(
			path_ / "OriginEngine/Engine/GameProject.json"
		);
		if (!ofs)
		{
			return false;
		}

		// パスをUTF-8に変換する
		const std::u8string utf8Path = currentProjectPath.generic_u8string();

		// JSONに渡すため、UTF-8のバイト列をstd::stringにする
		const std::string pathText(
			reinterpret_cast<const char*>(utf8Path.data()),
			utf8Path.size()
		);

		// JSONの構文・エスケープはライブラリに任せる
		json data;
		data["currentProjectPath"] = pathText;
		data["projectPaths"] = json::array();
		for (const auto& projectPath : projectPaths)
		{
			const std::u8string utf8ProjectPath = projectPath.rootPath.generic_u8string();
			const std::string projectPathText(
				reinterpret_cast<const char*>(utf8ProjectPath.data()),
				utf8ProjectPath.size()
			);
			data["projectPaths"].push_back(projectPathText);
		}

		ofs << data.dump(4);

		return static_cast<bool>(ofs);

		return true;
	}

	ProjectSaveData ProjectStorage::ProjectLoad()
	{

		ProjectSaveData saveData;
		if (!FindStandardFile())
		{
			return ProjectSaveData{};
		}
		namespace fs = std::filesystem;

		fs::path filePath =
			path_ / "OriginEngine/Engine/GameProject.json";

		std::ifstream ifs(filePath);

		if (!ifs)
		{
			return ProjectSaveData{};
		}
		try
		{
			json data = json::parse(ifs);

			// ① JSONがオブジェクトか確認
			if (!data.is_object())
			{
				return ProjectSaveData{};
			}
			// ② currentProjectPathキーが存在するか確認
			if (!data.contains("currentProjectPath"))
			{
				return ProjectSaveData{};
			}

			// ③ 文字列型か確認
			const auto& current = data["currentProjectPath"];

			if (current.is_string())
			{
				// パスへ変換
				// JSONの文字列はUTF-8
				const std::string projectPath =
					data["currentProjectPath"].get<std::string>();

				// UTF-8であることを明示してpathに変換する
				const std::u8string utf8Path(projectPath.begin(), projectPath.end());
				const fs::path fsProjectPath(utf8Path);

				saveData.currentProjectPath = fsProjectPath;
			}
			else if (!current.is_null())
			{
				return ProjectSaveData{};
			}
			

			// ⑤ projectPathsキーが存在するか確認
			if (!data.contains("projectPaths") ||
				!data["projectPaths"].is_array())
			{
				return ProjectSaveData{};
			}

			for (const auto& item : data["projectPaths"])
			{
				// JSONの要素が文字列か確認
				if (!item.is_string())
				{
					return ProjectSaveData{};
				}

				// JSONの文字列を取得
				std::string pathText = item.get<std::string>();

				// UTF-8文字列をfilesystem::pathに変換
				std::u8string utf8Path(
					pathText.begin(),
					pathText.end()
				);

				saveData.projectPaths.emplace_back(utf8Path);
			}
			// ⑥ vectorに追加して返す
			return saveData;
		}
		catch (const json::exception& e)
		{
			std::string errorMessage = e.what();
			return ProjectSaveData{};
		}

	}

	bool ProjectStorage::FindStandardFile()
	{
		PWSTR rawPath = nullptr;
		HRESULT hr = SHGetKnownFolderPath(
			FOLDERID_LocalAppData,
			0,
			nullptr,
			&rawPath
		);
		if (SUCCEEDED(hr))
		{
			path_ = rawPath;
			CoTaskMemFree(rawPath);
			return true;
		}
		return false;
	}
}