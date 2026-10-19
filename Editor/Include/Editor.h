#pragma once
#include "Include/EditorUI/DesignSupportPanel.h"
#include "Include/ImGuiRenderer/ImGuiRenderer.h"
class Graphics;
class Editor
{
public:
	explicit Editor(Graphics& graphics) :
		graphics_(graphics)
	{
	}
    bool Initialize();
    void Update();
    void Draw();
    void Shutdown();

private:
    Graphics& graphics_;
    ImGuiRenderer imguiRenderer_;
    DesignSupportPanel designSupport_;
};