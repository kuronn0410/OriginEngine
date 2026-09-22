#include "include/Scene/TestScene.h"

/*=== 仮実装 ===*/
#include <Windows.h>//GetAsyncKeyState
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Graphics/Color.h"
//#include "include/Graphics/Graphics.h"
#include "include/Scene/GameSceneType.h"
#include "include/Renderer/Renderer.h"

void TestScene::Render(Renderer& renderer)
{
    Color color{ 0.2f, 0.4f, 0.8f, 1.0f };
    renderer.Draw();
}

void TestScene::Update()
{
    if (GetAsyncKeyState('1') & 0x8000)
    {
        sceneRequest_.RequestChangeScene(GameSceneType::Title);
    }
}