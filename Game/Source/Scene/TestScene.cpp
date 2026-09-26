#include "include/Scene/TestScene.h"

/*=== 仮実装 ===*/
#include <Windows.h>//GetAsyncKeyState
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Graphics/Color.h"
#include "include/Graphics/Graphics.h"
#include "include/Scene/GameSceneType.h"
#include "include/Renderer/Renderer.h"
#include <cstdlib>

bool TestScene::Initialize(Graphics& graphics)
{
	mesh_.CreateVertexBuffer(*graphics.GetDevice(), vertices_, _countof(vertices_));
	mesh1_.CreateVertexBuffer(*graphics.GetDevice(), vertices1_, _countof(vertices1_));
    // 仮実装
    return true;
}

void TestScene::Render(Renderer& renderer)
{
    //Color color{ 0.2f, 0.4f, 0.8f, 1.0f };
    renderer.Draw(mesh_);
    renderer.Draw(mesh1_);
}

void TestScene::Update()
{
    if (GetAsyncKeyState('1') & 0x8000)
    {
        sceneRequest_.RequestChangeScene(GameSceneType::Title);
    }
}