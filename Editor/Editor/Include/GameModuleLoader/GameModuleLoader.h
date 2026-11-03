#pragma once
#include <Windows.h>
#include <filesystem>
#include "Include/Scenes/SceneChangeRequest.h"
#include "Include/Renderer/Resource/ResourceManager.h"  

namespace Origin::Editor
{
    class GameModuleLoader
    {
    public:
        bool Load(const std::filesystem::path& dllpath);
        void Unload();
        bool InitializeGame();
        void SetGameInitialize(SceneChangeRequest& sceneRequest, ResourceManager& resourceManager)
        {
            sceneRequest_ = &sceneRequest;
            resourceManager_ = &resourceManager;
        }
        void ShutdownGame();
        ~GameModuleLoader();

    private:
        HMODULE module_ = nullptr;
        SceneChangeRequest* sceneRequest_ = nullptr;
        ResourceManager* resourceManager_ = nullptr;
    };
}