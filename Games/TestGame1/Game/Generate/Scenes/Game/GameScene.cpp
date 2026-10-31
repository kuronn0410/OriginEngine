#include "GameScene.h"
#include "include/Renderer/Camera/Camera.h"
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Scene/GameSceneType.h"
#include "include/Renderer/Object/RenderObject.h"
#include "include/Renderer/Render/Renderer.h"
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Scene/GameResources.h"

namespace Game
{
	bool GameScene::Initialize(Origin::Graphics& graphics)
	{
	// 初期化処理
		/*--------Objectの初期化----------*/
		Origin::RenderObject renderObject;
		//renderObject.texture = gameResources_->groundTexture;
		//renderObject.mesh = gameResources_->CubeMesh;
		//renderObject.transform = {
			//{ 0.0f, -3.0f, -0.5f },//P
			//{ 0.0f, 0.0f, 0.0f },//R
			//{ 10.5f, 0.2f, 10.5f }//S
		//};
		//object_.SetRenderObject(renderObject);
		/*--------Cameraの初期化----------*/
		camera_.SetTransform({
			{ 0.0f, 0.0f, -5.0f },//P
			{ 0.0f, 0.0f, 0.0f },//R
		});
		camera_.SetProjectionSettings({
			60.0f,//fov
			1280.0f / 720.0f,//aspectRatio
			0.1f,//nearPlane
			100.0f//farPlane

		});
		return true;
	}

	void GameScene::Render(Origin::Renderer& renderer)
	{
	// 描画処理
		renderer.SetCamera(camera_);
		renderer.Draw(object_);
	}
	void GameScene::Update()
	{
	// 更新処理
	}

}
