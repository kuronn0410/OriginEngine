#include "include/Scene/TitleScene.h"
#include "include/Graphics/Color.h"
#include "include/Graphics/Graphics.h"

void TitleScene::Render(Graphics& graphics)
{
    Color color{ 1.0f, 1.0f, 0.0f, 1.0f };
    graphics.Render(color);
}

void TitleScene::Update()
{
    // ここに更新処理を書く
}