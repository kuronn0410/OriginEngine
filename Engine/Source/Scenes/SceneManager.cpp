#include "include/Scenes/SceneManager.h"
#include <memory>
#include <utility>
#include "include/Scenes/Scene.h"

void SceneManager::ChangeScene(std::unique_ptr<Scene> scene)
{
    currentScene_ = std::move(scene);
	currentScene_->Initialize(graphics_);
}

void SceneManager::Update()
{
    if (currentScene_)
    {
        currentScene_->Update();
    }
}

void SceneManager::Render(Renderer& renderer)
{
    if (currentScene_)
    {
        currentScene_->Render(renderer);
    }
}
