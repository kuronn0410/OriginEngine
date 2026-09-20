#pragma once
#include "include/Scene/GameSceneFactory.h"
class SceneChangeRequest;

class GameMain
{
public:
	bool Initialize(SceneChangeRequest& sceneRequest);
	void Finalize();
private:
	std::unique_ptr <GameSceneFactory> sceneFactory_;
};