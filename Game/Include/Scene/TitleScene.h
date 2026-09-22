#pragma once
#include "include/Scenes/Scene.h"
class SceneChangeRequest;
class Renderer;

class TitleScene : public Scene
{
public:
    TitleScene(SceneChangeRequest& sceneRequest) :
        Scene(sceneRequest)
    {
    }
    void Update() override;
    void Render(Renderer& renderer) override;

};