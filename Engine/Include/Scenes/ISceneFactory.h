#pragma once
#include <memory>
#include "include/Scenes/SceneType.h"

class Scene;
class SceneChangeRequest;

class ISceneFactory
{
public:
	ISceneFactory(SceneChangeRequest& sceneRequest):
		sceneRequest_(sceneRequest)
	{
	}
	virtual ~ISceneFactory() = default;
	virtual std::unique_ptr<Scene> CreateScene(SceneType type) = 0;
protected:
	SceneChangeRequest& sceneRequest_;
};