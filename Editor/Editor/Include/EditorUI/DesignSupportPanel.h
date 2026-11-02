#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/SceneCodeGenerator.h"
#include "Include/GameProject/ProjectManager.h"
namespace Origin::Editor
{
    class DesignSupportPanel : public IEditorPanel
    {
    public:
		DesignSupportPanel(ProjectManager& projectManager) :
			projectManager_(projectManager)
		{
		}
        void Draw() override;
    private:
        SceneCodeGenerator sceneCodeGenerator_;
        ProjectManager& projectManager_;
        char sceneName_[128] = {};
    };
}