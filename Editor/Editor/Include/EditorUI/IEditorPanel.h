#pragma once

namespace Origin::Editor
{
    class IEditorPanel
    {
    public:
        virtual ~IEditorPanel() = default;
		virtual bool Initialize() = 0;
        virtual void Draw() = 0;
    };
}