#pragma once
#include "Include/Scenes/ISceneFactory.h"
#include <memory>
#include "Include/Scene/GameResources.h"

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