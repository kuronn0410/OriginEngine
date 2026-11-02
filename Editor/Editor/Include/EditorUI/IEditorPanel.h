#pragma once

namespace Origin::Editor
{
    class IEditorPanel
    {
    public:
        virtual ~IEditorPanel() = default;

        virtual void Draw() = 0;
    };
}