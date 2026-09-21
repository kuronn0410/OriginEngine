#pragma once
#include "include/Scenes/Scene.h"
class SceneChangeRequest;

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