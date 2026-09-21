#include "include/Scenes/SceneChangeRequest.h"
#include "include/Scenes/Scene.h"
#include "include/Scenes/SceneManager.h"
#include "include/Scenes/ISceneFactory.h"
#include "include/Scenes/SceneType.h"
#include <memory>
#include <utility>



void SceneChangeRequest::SetSceneFactory(std::unique_ptr<ISceneFactory> sceneFactory)
{
	sceneFactory_  =  std::move(sceneFactory);
}

void SceneChangeRequest::RequestInitialScene(SceneType sceneType)
{
	std::unique_ptr<Scene> scene = sceneFactory_->CreateScene(sceneType);
	sceneManager_.ChangeScene(std::move(scene));
}

void SceneChangeRequest::RequestChangeScene(SceneType sceneType)
{
	std::unique_ptr<Scene> scene = sceneFactory_->CreateScene(sceneType);
	sceneManager_.ChangeScene(std::move(scene));
}
