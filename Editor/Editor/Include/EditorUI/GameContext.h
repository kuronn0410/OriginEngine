#pragma once
#include "Include/Renderer/Render/Renderer.h"
#include "Include/Scenes/SceneManager.h"
#include "Include/Graphics/OffscreenRenderTargetManager.h"

namespace Origin::Editor
{
    struct GameContext
    {
        Origin::Renderer* renderer = nullptr;
        Origin::SceneManager* sceneManager = nullptr;
		Origin::OffscreenRenderTargetManager* offscreenRenderTargetManager = nullptr;
    };
}