#pragma once
#include "include/Scenes/ISceneFactory.h"
#include <memory>
#include "include/Scene/GameResources.h"

namespace Origin
{
	class SceneChangeRequest;
	class Scene;
}

namespace Game
{

	class GameSceneFactory : public Origin::ISceneFactory
	{
	public:
		GameSceneFactory(Origin::SceneChangeRequest& sceneRequest, GameResources* gameResources) :
			Origin::ISceneFactory(sceneRequest),
			gameResources_(gameResources)
		{
		}

	private:
		virtual std::unique_ptr<Origin::Scene> CreateScene(Origin::SceneType type) override;
		GameResources* gameResources_ = nullptr;
	};
}