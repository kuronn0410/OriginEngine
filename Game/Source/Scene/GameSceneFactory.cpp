#include "include/Scene/GameSceneFactory.h"
#include "include/Scene/TestScene.h"
#include "include/Scene/TitleScene.h"
#include "include/Scenes/Scene.h"
#include "include/Scene/GameSceneType.h"
#include "include/Scenes/SceneType.h"
#include <memory>


std::unique_ptr<Scene> GameSceneFactory::CreateScene(SceneType type)
{
	std::unique_ptr<Scene> scene;
    switch (type)
    {
    case GameSceneType::Test:
		scene = std::make_unique<TestScene>(sceneRequest_, gameResources_);
        break;

    case GameSceneType::Title:
		scene = std::make_unique<TitleScene>(sceneRequest_, gameResources_);
        break;

    case GameSceneType::Game:
        // GameScene生成
        break;
    }

    return scene;
}