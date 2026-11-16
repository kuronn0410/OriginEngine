#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/EditorUI/GameContext.h"
namespace Origin::Editor
{
    class GameViewPanel : public IEditorPanel
    {
	public:
        GameViewPanel(GameContext& gameContext) :
            gameContext_(gameContext)
        {
        };
        bool Initialize() override;
        void Draw() override;
	private:
        GameContext& gameContext_;
    };
}