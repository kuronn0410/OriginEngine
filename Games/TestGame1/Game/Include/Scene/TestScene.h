#pragma once
#include "include/Scenes/Scene.h"
#include "include/Renderer/Object/Object.h"
#include "include/Renderer/Camera/Camera.h"
#include "include/Scene/GameResources.h"

namespace Origin
{
	class SceneChangeRequest;
	class Renderer;
	class Graphics;
}



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
		bool Initialize(Origin::Graphics& graphics);
		void Update() override;
		void Render(Origin::Renderer& renderer) override;
	private:
		Origin::Object object1_;
		Origin::Object object2_;
		Origin::Object object3_;
		Origin::Object wallobject1_;
		Origin::Object wallobject2_;
		Origin::Object wallobject3_;
		Origin::Object wallobject4_;
		Origin::Object wallobject5_;
		Origin::Camera camera_;
		GameResources* gameResources_ = nullptr;
	};
}