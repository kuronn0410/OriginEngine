#pragma once
#include "Include/EditorUI/IEditorPanel.h"

namespace Origin::Editor
{
    class ProjectPanel : public IEditorPanel
    {
    public:
        void Draw() override;
    private:
       
        char sceneName_[128] = {};
    };
}