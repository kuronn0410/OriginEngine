#pragma once
#include "include/Scenes/Scene.h"
class SceneChangeRequest;

class TitleScene : public Scene
{
public:
    TitleScene(SceneChangeRequest& sceneRequest) :
        Scene(sceneRequest)
    {
    }
    void Update() override;
    void Render(Graphics& graphics) override;

};