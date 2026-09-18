#pragma once
#include "include/Scenes/Scene.h"

class TitleScene : public Scene
{
public:
    void Update() override;
    void Render(Graphics& graphics) override;
};