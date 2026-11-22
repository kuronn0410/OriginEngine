#pragma once
namespace Origin::Editor
{
    class IOffscreenRenderable
    {
    public:
        virtual ~IOffscreenRenderable() = default;
        virtual void RenderOffscreen() = 0;
    };
}