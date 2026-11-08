#pragma once
#include "include/Scenes/Scene.h"
#include "include/Graphics/Graphics.h"
#include <memory>
namespace Origin
{
	class Renderer;
}


namespace Origin
{
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
		void ClearScene(){currentScene_.reset();}
	private:
		std::unique_ptr<Scene> currentScene_;
		Graphics& graphics_;
	};
}