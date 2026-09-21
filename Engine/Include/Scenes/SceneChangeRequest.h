#pragma once
#include <memory>
#include "include/Scenes/SceneType.h"
#include "include/Scenes/ISceneFactory.h"
#include "include/Scenes/SceneManager.h"
class Scene;


class SceneChangeRequest
{
public:
	// コンストラクタで SceneManager の参照を受け取る
	SceneChangeRequest(SceneManager& sceneManager):
		sceneManager_(sceneManager){}
	void SetSceneFactory(std::unique_ptr<ISceneFactory> sceneFactory);
	void RequestInitialScene(SceneType sceneType);
	void RequestChangeScene(SceneType sceneType);
private:
	SceneManager& sceneManager_;
	std::unique_ptr<ISceneFactory> sceneFactory_;
};