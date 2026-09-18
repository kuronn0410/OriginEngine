#include "include/Scenes/SceneManager.h"

void SceneManager::ChangeScene(std::unique_ptr<Scene> scene)
{
    currentScene_ = std::move(scene);
}

void SceneManager::Update()
{
    if (currentScene_)
    {
        currentScene_->Update();
    }
}

void SceneManager::Render(Graphics& graphics)
{
    if (currentScene_)
    {
        currentScene_->Render(graphics);
    }
}
