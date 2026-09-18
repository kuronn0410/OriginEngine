#include "include/Scenes/SceneChangeRequest.h"

void SceneChangeRequest::RequestInitialScene(std::unique_ptr<Scene> scene)
{
	sceneManager_.ChangeScene(std::move(scene));
}

void SceneChangeRequest::RequestChangeScene(std::unique_ptr<Scene> scene)
{
	sceneManager_.ChangeScene(std::move(scene));
}
