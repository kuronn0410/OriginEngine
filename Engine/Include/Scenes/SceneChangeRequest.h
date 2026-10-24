#pragma once
#include <memory>
#include "include/Scenes/SceneType.h"
#include "include/Scenes/ISceneFactory.h"
#include "include/Scenes/SceneManager.h"
class Scene;

namespace Origin
{
	class SceneChangeRequest
	{
	public:
		// コンストラクタで SceneManager の参照を受け取る
		SceneChangeRequest(Origin::SceneManager& sceneManager) :
			sceneManager_(sceneManager) {
		}
		void SetSceneFactory(std::unique_ptr<Origin::ISceneFactory> sceneFactory);
		void RequestInitialScene(Origin::SceneType sceneType);
		void RequestChangeScene(Origin::SceneType sceneType);
	private:
		SceneManager& sceneManager_;
		std::unique_ptr<ISceneFactory> sceneFactory_;
	};
}