#pragma once
#include "include/Scenes/Scene.h"
//#include "include/Graphics/Color.h"

class TestScene : public Scene
{
public:
    void Update() override;
    void Render(Graphics& graphics) override;
};