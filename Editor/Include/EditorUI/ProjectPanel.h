#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/GameProject/ProjectManager.h"
#include <array>

namespace Origin::Editor
{
    class ProjectPanel : public IEditorPanel
    {
    public:
		ProjectPanel(ProjectManager& projectManager):
			projectManager_(projectManager)
		{
		}
        void Draw() override;
    private:
		ProjectManager& projectManager_;
		std::array<char, 1024> projectPathBuffer_{};

    };
}