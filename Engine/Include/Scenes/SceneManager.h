#pragma once
#include "include/Scenes/Scene.h"
#include "include/Graphics/Graphics.h"
#include <memory>
class Renderer;

class SceneManager
{
public:
	SceneManager(Graphics& graphics) :
		graphics_(graphics)
	{
	}
	void Update();
	void ChangeScene(std::unique_ptr<Scene> scene);
	void Render(Renderer& renderer);
	
private:
    std::unique_ptr<Scene> currentScene_;
	Graphics& graphics_;
};