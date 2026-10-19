#pragma once
#pragma once

class IEditorPanel
{
public:
    virtual ~IEditorPanel() = default;

    virtual void Draw() = 0;
};