#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include "Include/SceneCodeGenerator.h"
class DesignSupportPanel : public IEditorPanel
{
public:
    void Draw() override;
private:
    SceneCodeGenerator sceneCodeGenerator_;
    char sceneName_[128] = {};
};