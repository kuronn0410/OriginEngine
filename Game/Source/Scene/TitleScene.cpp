#include "include/Scene/TitleScene.h"

void TitleScene::Render(Graphics& graphics)
{
    Color color{ 1.0f, 1.0f, 0.0f, 1.0f };
    graphics.Render(color);
}

void TitleScene::Update()
{
    // ここに更新処理を書く
}