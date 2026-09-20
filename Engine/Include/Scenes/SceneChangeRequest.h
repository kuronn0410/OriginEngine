#pragma once
#include <memory>

class SceneManager;
class Scene;

class SceneChangeRequest
{
public:
	// コンストラクタで SceneManager の参照を受け取る
	SceneChangeRequest(SceneManager& sceneManager):
		sceneManager_(sceneManager){}
	void RequestInitialScene(std::unique_ptr<Scene> scene);
	void RequestChangeScene(std::unique_ptr<Scene> scene);
private:
	SceneManager& sceneManager_;
};