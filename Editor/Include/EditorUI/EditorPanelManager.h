#pragma once
#include "Include/EditorUI/IEditorPanel.h"
#include <vector>
#include <memory>
#include <cstdint>

namespace Origin::Editor
{
    class EditorPanelManager
    {
    public:
		bool Initialize();
        void Draw();
        void DrawMenuBar();

    private:
		bool AddDrawPanel(uint32_t index);
        bool RemoveDrawPanel(uint32_t index);

        std::vector<std::unique_ptr<IEditorPanel>> panels_;
        std::vector<uint32_t> drawIndices_ = {};
    };
}