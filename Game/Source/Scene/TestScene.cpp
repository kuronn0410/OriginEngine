#include "include/Scene/TestScene.h"

/*=== 仮実装 ===*/
#include <Windows.h>//GetAsyncKeyState
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Scene/TitleScene.h"
#include "include/Graphics/Color.h"
#include "include/Graphics/Graphics.h"
#include "include/Scenes/SceneType.h"
#include "include/Scene/GameSceneFactory.h"
#include <memory>

void TestScene::Render(Graphics& graphics)
{
    Color color{ 0.2f, 0.4f, 0.8f, 1.0f };
    graphics.Render(color);
}

void TestScene::Update()
{
    if (GetAsyncKeyState('1') & 0x8000)
    {
        sceneRequest_.RequestChangeScene(std::make_unique<TitleScene>(sceneRequest_));
    }
}