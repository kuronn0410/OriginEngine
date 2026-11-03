#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/GameProject/ProjectManager.h"
#include "Include/Scenes/SceneChangeRequest.h"
#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/GameModuleLoader/GameModuleLoader.h"
#include <vector>
#include <memory>
#include <cstdint>

namespace Origin::Editor
{
    class EditorPanelManager
    {
    public:
		bool Initialize(ProjectManager& projectManager, GameModuleLoader& gameModuleLoader);
        void Draw();
        void DrawMenuBar();
		void GameInitialize(SceneChangeRequest& sceneRequest, ResourceManager& resourceManager);

    private:
		bool AddDrawPanel(uint32_t index);
        bool RemoveDrawPanel(uint32_t index);

        std::vector<std::unique_ptr<IEditorPanel>> panels_;
        std::vector<uint32_t> drawIndices_ = {};
    };
}