#pragma once
#include "include/Scenes/Scene.h"


class TestScene : public Scene
{
public:
    TestScene(SceneChangeRequest& sceneRequest):
        Scene(sceneRequest)
    {   
    }
    void Update() override;
    void Render(Graphics& graphics) override;

};