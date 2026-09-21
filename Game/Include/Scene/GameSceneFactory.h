#pragma once
#include "include/Scenes/ISceneFactory.h"
#include <memory>


class SceneChangeRequest;
class Scene;

class GameSceneFactory : public ISceneFactory
{
public:
	GameSceneFactory(SceneChangeRequest& sceneRequest) :
		ISceneFactory(sceneRequest)
	{
	}

private:
	virtual std::unique_ptr<Scene> CreateScene(SceneType type) override;
};