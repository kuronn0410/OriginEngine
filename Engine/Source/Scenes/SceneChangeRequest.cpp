#include "include/Scenes/SceneChangeRequest.h"

#include "include/Scenes/Scene.h"
#include "include/Scenes/SceneManager.h"


void SceneChangeRequest::RequestInitialScene(std::unique_ptr<Scene> scene)
{
	sceneManager_.ChangeScene(std::move(scene));
}

void SceneChangeRequest::RequestChangeScene(std::unique_ptr<Scene> scene)
{
	sceneManager_.ChangeScene(std::move(scene));
}
