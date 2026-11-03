#pragma once
#include <Windows.h>
#include <filesystem>
#include "Include/Scenes/SceneChangeRequest.h"
#include "Include/Renderer/Resource/ResourceManager.h"  
class GameModuleLoader
{
public:
    bool Load(const std::filesystem::path& dllpath);
    void Unload();
    bool InitializeGame(Origin::SceneChangeRequest& sceneRequest, Origin::ResourceManager& resourceManager);
    void ShutdownGame();
    ~GameModuleLoader();

private:
    HMODULE module_ = nullptr;
};