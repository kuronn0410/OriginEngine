#pragma once
#include "include/Scenes/Scene.h"
#include "include/Renderer/Object/Object.h"
#include "include/Renderer/Camera/Camera.h"
#include "include/Scene/GameResources.h"
namespace Origin
{
    class SceneChangeRequest;
}
class Renderer;
class Graphics;

namespace Game
{
    class TitleScene : public Origin::Scene
    {
    public:
        TitleScene(Origin::SceneChangeRequest& sceneRequest, GameResources* gameResources) :
            Scene(sceneRequest),
            gameResources_(gameResources)
        {
        }
        bool Initialize(Graphics& graphics);
        void Update() override;
        void Render(Renderer& renderer) override;
    private:
        Object object_;
        Camera camera_;
        GameResources* gameResources_ = nullptr;
    };
}