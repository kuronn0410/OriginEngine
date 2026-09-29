#pragma once
#include "include/Scenes/ISceneFactory.h"
#include <memory>
#include "include/Scene/GameResources.h"


class SceneChangeRequest;
class ResourceManager;
class Scene;

class GameSceneFactory : public ISceneFactory
{
public:
	GameSceneFactory(SceneChangeRequest& sceneRequest, GameResources* gameResources) :
		ISceneFactory(sceneRequest),
		gameResources_(gameResources)
	{
	}

private:
	virtual std::unique_ptr<Scene> CreateScene(SceneType type) override;
	GameResources* gameResources_ = nullptr;
};