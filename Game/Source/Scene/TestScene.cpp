#include "include/Scene/TestScene.h"
#include <Windows.h>//GetAsyncKeyState
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Graphics/Graphics.h"
#include "include/Scene/GameSceneType.h"
#include "include/Renderer/Render/Renderer.h"
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Scene/GameResources.h"
#include "include/Renderer/Object/RenderObject.h"

namespace Game
{ 
	bool TestScene::Initialize(Origin::Graphics& graphics)
	{
		/*------Objectの初期化------*/
		//1
		Origin::RenderObject renderObject1;
		renderObject1.mesh = gameResources_->playerMesh;
		renderObject1.texture = gameResources_->backgroundTexture;
		renderObject1.transform ={
		{ 0.0f, -3.0f, 0.0f },//P
		{ 0.0f, 90.0f, 0.0f },//R
		{ 0.5f, 0.5f, 0.5f }//S
		};
		object1_.SetRenderObject(renderObject1);

		//2
		Origin::RenderObject renderObject2;
		renderObject2.mesh = gameResources_->playerMesh;
		renderObject2.texture = gameResources_->baseTexture;
		renderObject2.transform = {
			{ 0.0f, -3.0f, 2.0f },//P
			{ 0.0f, 0.0f, 0.0f },//R
			{ 0.5f, 0.5f, 0.5f }//S
		};
		object2_.SetRenderObject(renderObject2);

		//3
		Origin::RenderObject renderObject3;
		renderObject3.texture = gameResources_->groundTexture;
		renderObject3.mesh = gameResources_->CubeMesh;
		renderObject3.transform = {
			{ 0.0f, -3.0f, -0.5f },//P
			{ 0.0f, 0.0f, 0.0f },//R
			{ 10.5f, 0.2f, 10.5f }//S
		};
		object3_.SetRenderObject(renderObject3);

		//4
		Origin::RenderObject wallrenderObject;
		wallrenderObject.texture = gameResources_->rockTexture;
		wallrenderObject.mesh = gameResources_->CubeMesh;
		wallrenderObject.transform = {
			{ -4.0f, 0.0f, 0.5f },//P
			{ 0.0f, 0.0f, 0.0f },//R
			{ 0.2f, 5.0f, 1.0f }//S
		};
		wallobject1_.SetRenderObject(wallrenderObject);

		//5
		wallrenderObject.transform = {
			{ 4.0f, 0.0f, 0.5f },//P
			{ 0.0f, 0.0f, 0.0f },//R
			{ 0.2f, 5.0f, 1.0f }//S
		};
		wallobject2_.SetRenderObject(wallrenderObject);

		//6
		wallrenderObject.transform = {
			{ 0.0f, 0.0f, 12.0f },//P
			{ 0.0f, 0.0f, 0.0f },//R
			{ 8.0f, 5.0f, 0.2f }//S
		};
		wallobject3_.SetRenderObject(wallrenderObject);

		//7
		wallrenderObject.transform = {
			{ 25.0f, 0.0f, 6.0f },//P
			{ 0.0f, 0.0f, 0.0f },//R
			{ 4.0f, 5.0f, 0.2f }//S
		};
		wallobject4_.SetRenderObject(wallrenderObject);

		//8
		wallrenderObject.transform = {
			{ -25.0f, 0.0f, 6.0f },//P
			{ 0.0f, 0.0f, 0.0f },//R
			{ 4.0f, 5.0f, 0.2f }//S
		};
		wallobject5_.SetRenderObject(wallrenderObject);

		/*--------Cameraの初期化----------*/
		camera_.SetTransform({
			{ 0.0f, 0.0f, -5.0f },
			{ 0.0f, 0.0f, 0.0f }//縦、横
			});

		camera_.SetProjectionSettings({
			60.0f,//fov
			1280.0f / 720.0f,//aspectRatio
			0.1f,//nearPlane
			100.0f//farPlane
			});

		// 仮実装
		return true;
	}

	void TestScene::Render(Origin::Renderer& renderer)
	{
	   renderer.SetCamera(camera_);

	   renderer.Draw(object1_);
	   renderer.Draw(object2_);
	   renderer.Draw(object3_);
	   renderer.Draw(wallobject1_);
	   renderer.Draw(wallobject2_);
	   renderer.Draw(wallobject3_);
	   renderer.Draw(wallobject4_);
	   renderer.Draw(wallobject5_);
	}

	void TestScene::Update()
	{
		/*----Cameraの更新------*/
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			object1_.GetRenderObject().transform.rotation.y += 0.3f;
			camera_.GetTransform().rotation.y += 0.3f;
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			object1_.GetRenderObject().transform.rotation.y -= 0.3f;
			camera_.GetTransform().rotation.y -= 0.3f;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			camera_.GetTransform().position.y += 0.03f;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			camera_.GetTransform().position.y -= 0.03f;
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			camera_.GetTransform().rotation.x += 0.1f;
		}
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			camera_.GetTransform().rotation.x -= 0.1f;
		}

		//移動
		if (GetAsyncKeyState('W') & 0x8000)
		{
			object1_.GetRenderObject().transform.position.z += 0.1f;
			camera_.GetTransform().position.z += 0.1f;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			object1_.GetRenderObject().transform.position.z -= 0.1f;
			camera_.GetTransform().position.z -= 0.1f;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			object1_.GetRenderObject().transform.position.x -= 0.1f;
			camera_.GetTransform().position.x -= 0.1f;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			object1_.GetRenderObject().transform.position.x += 0.1f;
			camera_.GetTransform().position.x += 0.1f;
		}

		if (GetAsyncKeyState('1') & 0x8000)
		{
			sceneRequest_.RequestChangeScene(GameSceneType::Title);
		}
	}

}