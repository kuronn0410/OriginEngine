#include "Include/EditorUI/ProjectPanel.h"
#include "External/imgui/imgui.h"
#include <system_error>
#include <Windows.h>
namespace Origin::Editor
{
	bool ProjectPanel::Initialize()
	{
		loadedProjects_ = &projectManager_.GetLoadedProjects();
		return true;
	}

    void ProjectPanel::Draw()
    {
        namespace fs = std::filesystem;

        ImGui::Begin("OriginEngine Editor##ProjectPanel");

        // ========================================
        // 1. 現在のプロジェクト
        // ========================================
        ImGui::SeparatorText("Current Project");

        const auto* currentProject =
            projectManager_.GetCurrentProject();

        if (currentProject)
        {
            ImGui::Text("Name: %s",
                currentProject->name.c_str());

            const std::u8string utf8Path =
                currentProject->rootPath.generic_u8string();

            const char* pathText =
                reinterpret_cast<const char*>(utf8Path.c_str());

            ImGui::TextDisabled("Path: %s", pathText);
        }
        else
        {
            ImGui::TextDisabled("No project selected.");
        }

        ImGui::Spacing();
        ImGui::Spacing();

        // ========================================
        // 2. 既存プロジェクトの登録
        // ========================================
        ImGui::SeparatorText("Add Existing Project");

        ImGui::TextDisabled("Enter the project directory.");

        ImGui::SetNextItemWidth(-1);

        ImGui::InputText(
            "##ProjectPath",
            projectPathBuffer_.data(),
            projectPathBuffer_.size()
        );

        ImGui::Spacing();

        if (ImGui::Button("Add Project", ImVec2(180, 36)))
        {
            std::u8string utf8Path(
                reinterpret_cast<const char8_t*>(
                    projectPathBuffer_.data())
            );

            fs::path directory(utf8Path);

            if (!projectManager_.InitialRegistration(directory))
            {
                // TODO: エラーメッセージを表示
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();

        // ========================================
        // 3. 登録済みプロジェクト一覧
        // ========================================
        ImGui::SeparatorText("Registered Projects");

        std::optional<fs::path> requestedProject;

        if (ImGui::BeginChild(
            "ProjectList",
            ImVec2(0, 220),
            true))
        {
            if (loadedProjects_ && !loadedProjects_->empty())
            {
                for (const auto& project : *loadedProjects_)
                {
                    const std::u8string utf8Path =
                        project.rootPath.generic_u8string();

                    const char* pathText =
                        reinterpret_cast<const char*>(utf8Path.c_str());

                    ImGui::PushID(pathText);

                    // 現在のプロジェクトと一致すれば選択状態
                    const bool isSelected =
                        currentProject &&
                        currentProject->rootPath == project.rootPath;

                    if (ImGui::Selectable(
                        project.name.c_str(),
                        isSelected,
                        0,
                        ImVec2(0, 32)))
                    {
                        requestedProject = project.rootPath;
                    }

                    ImGui::TextDisabled("%s", pathText);

                    ImGui::Separator();
                    ImGui::PopID();
                }
            }
            else
            {
                ImGui::TextDisabled(
                    "No registered projects.");
            }
        }

        ImGui::EndChild();

        // 一覧の描画完了後に切り替える
        if (requestedProject)
        {
            projectManager_.LoadProject(*requestedProject);
        }

        ImGui::Spacing();
        ImGui::Spacing();

        // ========================================
        // 4. ゲームプロジェクトを開く
        // ========================================
        ImGui::SeparatorText("Launch Project");

        const auto* selectedProject =
            projectManager_.GetCurrentProject();

        ImGui::BeginDisabled(selectedProject == nullptr);

        if (ImGui::Button("Open Game", ImVec2(-1, 42)))
        {
            if (selectedProject)
            {
                if (gameModuleLoader_.Load(
                    selectedProject->gameDllPath))
                {
                    // InitializeGame()の戻り値仕様は要確認
                    gameModuleLoader_.InitializeGame();
                }
                else
                {
                    // TODO: DLL読み込み失敗の表示
                }
            }
        }

        ImGui::EndDisabled();

        ImGui::End();
    }

}