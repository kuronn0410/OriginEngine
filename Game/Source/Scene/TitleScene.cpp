#include "include/Scene/TitleScene.h"
#include "include/Graphics/Color.h"
#include "include/Graphics/Graphics.h"
#include "include/Scene/GameSceneType.h"
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Renderer/Renderer.h"
#include <Windows.h>//GetAsyncKeyState
#include <cstdlib>

bool TitleScene::Initialize(Graphics& graphics)
{
	mesh_.CreateVertexBuffer(*graphics.GetDevice(), vertices_, _countof(vertices_));
	// 仮実装
	return true;
}

void TitleScene::Render(Renderer& renderer)
{
    //Color color{ 1.0f, 1.0f, 0.0f, 1.0f };
    renderer.Draw(mesh_);
}

void TitleScene::Update()
{
    // ここに更新処理を書く
    if (GetAsyncKeyState('2') & 0x8000)
    {
        sceneRequest_.RequestChangeScene(GameSceneType::Test);
    }
}