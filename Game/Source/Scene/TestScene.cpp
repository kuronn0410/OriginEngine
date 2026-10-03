#include "include/Scene/TestScene.h"
#include <Windows.h>//GetAsyncKeyState
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Graphics/Graphics.h"
#include "include/Scene/GameSceneType.h"
#include "include/Renderer/Render/Renderer.h"
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Scene/GameResources.h"
#include "include/Renderer/Object/RenderObject.h"

bool TestScene::Initialize(Graphics& graphics)
{
	/*------Objectの初期化------*/
    //1
	RenderObject renderObject1;
    renderObject1.mesh = gameResources_->playerMesh;
    renderObject1.texture = gameResources_->backgroundTexture;
    renderObject1.transform ={
	{ 0.0f, -3.0f, 0.0f },//P
	{ 0.0f, 0.0f, 0.0f },//R
	{ 0.5f, 0.5f, 0.5f }//S
	};
    object1_.SetRenderObject(renderObject1);

    //2
	RenderObject renderObject2;
	renderObject2.mesh = gameResources_->playerMesh;
    renderObject2.texture = gameResources_->baseTexture;
	renderObject2.transform = {
		{ 0.0f, -3.0f, 2.0f },//P
		{ 0.0f, 0.0f, 0.0f },//R
		{ 0.5f, 0.5f, 0.5f }//S
	};
	object2_.SetRenderObject(renderObject2);

    //3
	RenderObject renderObject3;
	renderObject3.texture = gameResources_->baseTexture;
    renderObject3.mesh = gameResources_->treeMesh;
	renderObject3.transform = {
		{ 0.0f, 0.0f, -0.5f },//P
		{ 0.0f, 0.0f, 0.0f },//R
		{ 0.5f, 0.5f, 0.5f }//S
	};
	object3_.SetRenderObject(renderObject3);

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

void TestScene::Render(Renderer& renderer)
{
   renderer.SetCamera(camera_);


   renderer.Draw(object1_);
   renderer.Draw(object2_);
}

void TestScene::Update()
{
    /*----Object1の更新------*/
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        object1_.GetRenderObject().transform.position.x += 0.01f;
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        object1_.GetRenderObject().transform.position.x -= 0.01f;
    }

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        object1_.GetRenderObject().transform.position.z += 0.01f;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        object1_.GetRenderObject().transform.position.z -= 0.01f;
    }

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        object1_.GetRenderObject().transform.rotation.y += 0.5f;
    }
    ///*----Object2の更新------*/
    if (GetAsyncKeyState('W') & 0x8000)
    {
        object2_.GetRenderObject().transform.position.z += 0.01f;
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        object2_.GetRenderObject().transform.position.z -= 0.01f;
    }
    if (GetAsyncKeyState('A') & 0x8000)
    {
        object2_.GetRenderObject().transform.position.x -= 0.01f;
    }
    if (GetAsyncKeyState('D') & 0x8000)
    {
        object2_.GetRenderObject().transform.position.x += 0.01f;
    }

    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
    {
        object2_.GetRenderObject().transform.rotation.y += 0.5f;
    }

    if (GetAsyncKeyState('1') & 0x8000)
    {
        sceneRequest_.RequestChangeScene(GameSceneType::Title);
    }
}