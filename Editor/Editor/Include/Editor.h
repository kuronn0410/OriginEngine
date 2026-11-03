#pragma once
#include "Include/EditorUI/DesignSupportPanel.h"
#include "Include/ImGuiRenderer/ImGuiRenderer.h"
#include  "Include/Graphics/Graphics.h"
#include "Include/Window/WindowManager.h"
#include "Include/Window/WindowHandle.h"
#include <vector>
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/GameProject/ProjectManager.h"
#include "Include/EditorUI/EditorPanelManager.h" 
#include "Include/GameModuleLoader/GameModuleLoader.h"
#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/Scenes/SceneChangeRequest.h"

namespace Origin::Editor
{
    class Editor
    {
    public:
        explicit Editor(Graphics& graphics, WindowManager& windowManager) :
            graphics_(graphics),
            imguiRenderer_(
                *graphics.GetDescriptorAllocator(),
                *graphics.GetSRVHeap()

            ),
            windowManager_(windowManager)
        {
        }
        bool Initialize(
            int numFramesInFlight,
            DXGI_FORMAT rtvFormat,
            //ウィンドウ側
            WindowHandle windowHandle);
		//ゲーム側の初期化するために必要なクラスの参照を受け取る
		void GameInitialize(SceneChangeRequest& sceneRequest, ResourceManager& resourceManager);
        void Update();
        void Draw();
        void Shutdown();

    private:
        Graphics& graphics_;
        ImGuiRenderer imguiRenderer_;
        WindowManager& windowManager_;
        WindowHandle windowHandle_;
        ProjectManager projectManager_;
        EditorPanelManager editorPanelManager_;
		GameModuleLoader gameModuleLoader_;
    };
}