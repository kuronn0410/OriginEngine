#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/GameProject/ProjectManager.h"
#include "Include/GameModuleLoader/GameModuleLoader.h"
#include <array>

namespace Origin::Editor
{
    class ProjectPanel : public IEditorPanel
    {
    public:
		ProjectPanel(ProjectManager& projectManager, GameModuleLoader& gameModuleLoader):
			projectManager_(projectManager),
			gameModuleLoader_(gameModuleLoader)
		{
		}
        void Draw() override;
    private:
		ProjectManager& projectManager_;
		GameModuleLoader& gameModuleLoader_;
		std::array<char, 1024> projectPathBuffer_{};

    };
}