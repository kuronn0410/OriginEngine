#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/EditorUI/GameContext.h"
#include "Include/Graphics/Graphics.h"
namespace Origin::Editor
{
    class GameViewPanel : public IEditorPanel
    {
	public:
        GameViewPanel(GameContext& gameContext, Graphics& graphics) :
            gameContext_(gameContext),
            graphics_(graphics)
        {
        };
        bool Initialize() override;
        void Draw() override;
	private:
        GameContext& gameContext_;
        Graphics& graphics_;
    };
}