#pragma once
#include "include/Scenes/Scene.h"
#include <memory>

class Graphics;

class SceneManager
{
public:
	void Update();
	void ChangeScene(std::unique_ptr<Scene> scene);
	void Render(Graphics& graphics);
	
private:
    std::unique_ptr<Scene> currentScene_;
};