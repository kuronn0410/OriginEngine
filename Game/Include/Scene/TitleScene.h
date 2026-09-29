#pragma once
#include "include/Scenes/Scene.h"
#include "include/Renderer/Mesh.h"
#include "include/Renderer/Vertex.h"
#include "include/Renderer/Object.h"
class SceneChangeRequest;
class Renderer;
class Graphics;
class GameResources;

class TitleScene : public Scene
{
public:
    TitleScene(SceneChangeRequest& sceneRequest,GameResources* gameResources) :
        Scene(sceneRequest),
        gameResources_(gameResources)
    {
    }
    bool Initialize(Graphics& graphics);
    void Update() override;
    void Render(Renderer& renderer) override;
private:
	Object object_;
	GameResources* gameResources_ = nullptr;
};