#pragma once
#include "include/Scenes/Scene.h"
#include "include/Renderer/Object/Object.h"
#include "include/Renderer/Camera/Camera.h"
#include "include/Scene/GameResources.h"

namespace Origin
{
	class SceneChangeRequest;
}

class Renderer;
class Graphics;
//class GameResources;

namespace Game
{
	class TestScene : public Origin::Scene
	{
	public:
		TestScene(Origin::SceneChangeRequest& sceneRequest, GameResources* gameResources) :
			Scene(sceneRequest),
			gameResources_(gameResources)
		{
		}
		bool Initialize(Graphics& graphics);
		void Update() override;
		void Render(Renderer& renderer) override;
	private:
		Object object1_;
		Object object2_;
		Object object3_;
		Object wallobject1_;
		Object wallobject2_;
		Object wallobject3_;
		Object wallobject4_;
		Object wallobject5_;
		Camera camera_;
		GameResources* gameResources_ = nullptr;
	};
}