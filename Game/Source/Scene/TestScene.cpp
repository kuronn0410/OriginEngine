#include "include/Scene/TestScene.h"

/*=== 仮実装 ===*/
#include <Windows.h>//GetAsyncKeyState
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Graphics/Graphics.h"
#include "include/Scene/GameSceneType.h"
#include "include/Renderer/Renderer.h"
#include "include/Renderer/MeshHandle.h"
#include "include/Scene/GameResources.h"

bool TestScene::Initialize(Graphics& graphics)
{
	MeshHandle meshHandle1_ = gameResources_->playerMesh;
	object1_.SetTransform({ 0.7f, 0.0f, 0.0f });
	object1_.SetMeshHandle(meshHandle1_);

    MeshHandle meshHandle2_ = gameResources_->enemyMesh;
    object2_.SetTransform({ 0.5f, 0.0f, 0.0f });
    object2_.SetMeshHandle(meshHandle2_);

    // 仮実装
    return true;
}

void TestScene::Render(Renderer& renderer)
{
   renderer.Draw(object1_);
   renderer.Draw(object2_);
}

void TestScene::Update()
{
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        object2_.GetTransform().position.x += 0.01f;
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        object2_.GetTransform().position.x -= 0.01f;
    }

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        object1_.GetTransform().position.y += 0.01f;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        object1_.GetTransform().position.y -= 0.01f;
    }

    if (GetAsyncKeyState('1') & 0x8000)
    {
        sceneRequest_.RequestChangeScene(GameSceneType::Title);
    }
}