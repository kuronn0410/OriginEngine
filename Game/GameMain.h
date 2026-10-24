#pragma once
#include <memory>
#include "include/Scene/GameSceneFactory.h"
#include "include/Scenes/ISceneFactory.h"
#include "include/Scene/GameResources.h"
#include "include/Renderer/Mesh/MeshData.h"
namespace Origin
{
	class SceneChangeRequest;
}
class ResourceManager;

namespace Game
{
	class GameMain
	{
	public:
		bool Initialize(Origin::SceneChangeRequest& sceneRequest, ResourceManager& resourceManager);
		void Finalize();
	private:
		std::unique_ptr<Origin::ISceneFactory> sceneFactory_;

		MeshData meshData1_;
		MeshData meshData2_;
		MeshData meshData3_;
		GameResources gameResources_;
	};
}