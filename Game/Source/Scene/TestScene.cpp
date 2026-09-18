#include "include/Scene/TestScene.h"

void TestScene::Render(Graphics& graphics)
{
    Color color{ 0.2f, 0.4f, 0.8f, 1.0f };
    graphics.Render(color);
}

void TestScene::Update()
{
	// ここに更新処理を書く
}