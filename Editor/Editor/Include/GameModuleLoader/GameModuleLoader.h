#pragma once
#include <Windows.h>
#include <filesystem>
#include <optional>
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
        void RequestOpen(const std::filesystem::path& dllPath)
        {
            requestedDllPath_ = dllPath; // パスをコピーして保持
        }

        bool HasOpenRequest() const
        {
            return requestedDllPath_.has_value();
        }

        bool ProcessOpenRequest();
        void ShutdownGame();
        ~GameModuleLoader();

    private:
        std::optional<std::filesystem::path> requestedDllPath_;
        HMODULE module_ = nullptr;
        SceneChangeRequest* sceneRequest_ = nullptr;
        ResourceManager* resourceManager_ = nullptr;
    };
}