#pragma once
#include <memory>
#include "include/Scene/GameSceneFactory.h"
#include "include/Scenes/ISceneFactory.h"
#include "include/Scene/GameResources.h"
#include "include/Renderer/Mesh/MeshData.h"
class SceneChangeRequest;
class ResourceManager;

class GameMain
{
public:
	bool Initialize(SceneChangeRequest& sceneRequest, ResourceManager& resourceManager);
	void Finalize();
private:
	std::unique_ptr<ISceneFactory> sceneFactory_;

	MeshData meshData1_;
	MeshData meshData2_;
	MeshData meshData3_;
	GameResources gameResources_;
};