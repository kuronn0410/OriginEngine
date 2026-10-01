#include "include/Scene/TestScene.h"

/*=== 仮実装 ===*/
#include <Windows.h>//GetAsyncKeyState
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Graphics/Graphics.h"
#include "include/Scene/GameSceneType.h"
#include "include/Renderer/Render/Renderer.h"
#include "include/Renderer/Mesh/MeshHandle.h"
#include "include/Scene/GameResources.h"

bool TestScene::Initialize(Graphics& graphics)
{
	/*------Objectの初期化------*/
	MeshHandle meshHandle1_ = gameResources_->playerMesh;
    object1_.SetTransform({
		{ 0.0f, -3.0f, 0.0f },//P
		{ 0.0f, 0.0f, 0.0f },//R
        { 0.5f, 0.5f, 0.5f }//S
    });

    float z = object1_.GetTransform().rotation.z;
	object1_.SetMeshHandle(meshHandle1_);

    MeshHandle meshHandle2_ = gameResources_->treeMesh;
    object2_.SetTransform({ 
        { 0.0f, -0.9f, 0.8f },
        { 0.0f, -10.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f } 
        });
    object2_.SetMeshHandle(meshHandle2_);

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
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        object1_.GetTransform().position.x += 0.01f;
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        object1_.GetTransform().position.x -= 0.01f;
    }

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        object1_.GetTransform().position.z += 0.01f;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        object1_.GetTransform().position.z -= 0.01f;
    }

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        object1_.GetTransform().rotation.y += 0.5f;
    }

    if (GetAsyncKeyState('1') & 0x8000)
    {
        sceneRequest_.RequestChangeScene(GameSceneType::Title);
    }
}