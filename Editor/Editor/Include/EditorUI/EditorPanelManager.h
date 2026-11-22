#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/GameProject/ProjectManager.h"
#include "Include/Scenes/SceneChangeRequest.h"
#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/GameModuleLoader/GameModuleLoader.h"
#include "Include/EditorUI/GameContext.h"
#include "Include/Graphics/OffscreenRenderTargetManager.h"
#include "Include/EditorUI/IOffscreenRenderable.h"
#include "External/imgui/imgui.h"
#include <vector>
#include <memory>
#include <cstdint>

namespace Origin::Editor
{
    class EditorPanelManager
    {
    public:
		bool Initialize(
            ProjectManager& projectManager, 
            GameModuleLoader& gameModuleLoader
        );
		bool AddGamePanel(GameContext& gameContext, Graphics& graphics);
        void Draw();
        void DrawMenuBar();
		void GameInitialize(
            SceneChangeRequest& sceneRequest, 
            ResourceManager& resourceManager
        );
        void RenderOffscreen();
    private:
		bool AddDrawPanel(uint32_t index);
        bool RemoveDrawPanel(uint32_t index);
        void SetupDefaultLayout(ImGuiID dockspaceId);
        std::vector<std::unique_ptr<IEditorPanel>> panels_;
        std::vector<uint32_t> drawIndices_ = {};
        std::vector<IOffscreenRenderable*> offscreenPanels_ = {};
		bool isDefaultLayoutSetup_ = false;
    };
}