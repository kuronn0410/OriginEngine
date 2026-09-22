#pragma once
#include "include/Scenes/Scene.h"
#include <memory>

//class Graphics;
class Renderer;

class SceneManager
{
public:
	void Update();
	void ChangeScene(std::unique_ptr<Scene> scene);
	void Render(Renderer& renderer);
	
private:
    std::unique_ptr<Scene> currentScene_;
};