#pragma once
#include <memory>
#include "include/Scene/GameSceneFactory.h"
#include "include/Scenes/ISceneFactory.h"
class SceneChangeRequest;

class GameMain
{
public:
	bool Initialize(SceneChangeRequest& sceneRequest);
	void Finalize();
private:
	std::unique_ptr<ISceneFactory> sceneFactory_;
};