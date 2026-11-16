#pragma once
#include "Include/Renderer/Render/Renderer.h"
#include "Include/Scenes/SceneManager.h"

namespace Origin::Editor
{
    struct GameContext
    {
        Origin::Renderer* renderer = nullptr;
        Origin::SceneManager* sceneManager = nullptr;
    };
}