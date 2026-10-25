#pragma once
#include <memory>
#include "Include/Scenes/SceneType.h"

namespace Origin
{
	class SceneChangeRequest;
	class Scene;
}

namespace Origin
{
	class ISceneFactory
	{
	public:
		ISceneFactory(SceneChangeRequest& sceneRequest) :
			sceneRequest_(sceneRequest)
		{
		}
		virtual ~ISceneFactory() = default;
		virtual std::unique_ptr<Scene> CreateScene(SceneType type) = 0;
	protected:
		SceneChangeRequest& sceneRequest_;
	};
}