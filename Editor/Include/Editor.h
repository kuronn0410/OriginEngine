#pragma once
#include "Include/EditorUI/DesignSupportPanel.h"
#include "Include/ImGuiRenderer/ImGuiRenderer.h"
#include  "Include/Graphics/Graphics.h"
class Graphics;
class Editor
{
public:
    explicit Editor(Graphics& graphics) :
        graphics_(graphics),
        imguiRenderer_(
            *graphics.GetDescriptorAllocator(),
            *graphics.GetSRVHeap()
        )
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