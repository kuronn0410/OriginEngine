#pragma once
#include "include/Scenes/ISceneFactory.h"
#include "include/Scene/GameSceneType.h"


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
	//SceneChangeRequest& sceneRequest_;
};