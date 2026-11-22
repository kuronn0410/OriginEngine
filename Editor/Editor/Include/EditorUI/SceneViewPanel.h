#pragma once
#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/EditorUI/GameContext.h"
#include "Include/Graphics/Graphics.h"
#include "Include/Graphics/OffscreenRenderTargetManager.h"
#include "Include/Graphics/OffscreenHandle.h" 
#include "Include/EditorUI/IOffscreenRenderable.h"
namespace Origin::Editor
{
    class SceneViewPanel :
        public IEditorPanel,
        public IOffscreenRenderable
    {
    public:
        SceneViewPanel(GameContext& gameContext, Graphics& graphics) :
            gameContext_(gameContext),
            graphics_(graphics)
        {
        };

        bool Initialize() override;
        void Draw() override;
        void RenderOffscreen() override;
    private:
        GameContext& gameContext_;
        OffscreenHandle offscreenHandle_;
        Graphics& graphics_;
    };
}