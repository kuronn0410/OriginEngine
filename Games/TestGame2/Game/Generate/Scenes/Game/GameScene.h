#pragma once

#include "Include/Scenes/Scene.h"
#include "include/Renderer/Camera/Camera.h"
#include "include/Scene/GameResources.h"

#include "include/Renderer/Object/Object.h"

namespace Origin
{
	class SceneChangeRequest;
	class Renderer;
	class Graphics;
}

namespace Game
{
	class GameScene : public Origin::Scene
	{
	public:
		GameScene(
			Origin::SceneChangeRequest& sceneRequest,
			GameResources* gameResources
		) :
			Scene(sceneRequest),
			gameResources_(gameResources)
		{
		}
		bool Initialize(Origin::Graphics& graphics);
		void Update()override;
		void Render(Origin::Renderer& renderer);
	private:
		Origin::Object object_;
		Origin::Camera camera_;
		GameResources* gameResources_ = nullptr;
	};
}
