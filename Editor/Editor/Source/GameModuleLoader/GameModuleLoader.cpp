#include "Include/GameModuleLoader/GameModuleLoader.h"
#include "Include/Scenes/SceneChangeRequest.h"
#include "Include/Renderer/Resource/ResourceManager.h"

namespace Origin::Editor
{
    bool GameModuleLoader::Load(const std::filesystem::path& dllpath)
    {
        //もし既にロード済みのDLLがある場合はfalseを返す
        if (module_ != nullptr)
        {
            return false;
        }
        module_ = LoadLibraryW(dllpath.c_str());

        if (module_ == nullptr)
        {
            DWORD error = GetLastError(); // ブレークポイント
            return false;
        }
        FARPROC address = GetProcAddress(
            module_,
            "GetGameVersion"
        );
        if (!address)
        {
            FreeLibrary(module_);
            module_ = nullptr;
            return false;
        }
        using GetGameVersionFunc = int (*)();
        GetGameVersionFunc getVersion =
            reinterpret_cast<GetGameVersionFunc>(address);

        int version = getVersion();
        return true;
    }



    bool GameModuleLoader::InitializeGame()
    {
        if (!module_)
        {
            return false;
        }

        if (sceneRequest_ == nullptr || resourceManager_ == nullptr)
        {
			return false;
        }

        FARPROC address = GetProcAddress(module_, "InitializeGame");
        if (!address)
        {
            return false;
        }

        using InitializeGameFunc = bool (*)(
            Origin::SceneChangeRequest&,
            Origin::ResourceManager&
            );

        auto initialize =
            reinterpret_cast<InitializeGameFunc>(address);

        return initialize(*sceneRequest_, *resourceManager_);
    }

    void GameModuleLoader::ShutdownGame()
    {
        if (!module_)
        {
            return;
        }

        FARPROC address = GetProcAddress(
            module_,
            "ShutdownGame"
        );
        if (!address)
        {
            return;
        }

        using ShutdownGameFunc = void (*)();
        ShutdownGameFunc shutdown =
            reinterpret_cast<ShutdownGameFunc>(address);

        shutdown();
    }

    void GameModuleLoader::Unload()
    {
        if (module_)
        {
            FreeLibrary(module_);
            module_ = nullptr;
        }
    }

    GameModuleLoader::~GameModuleLoader()
    {
        Unload();
    }
    bool GameModuleLoader::ProcessOpenRequest()
    {
        if (!requestedDllPath_)
        {
            return true;
        }

        const std::filesystem::path dllPath = *requestedDllPath_;
        requestedDllPath_.reset();

        if (!sceneRequest_ || !resourceManager_)
        {
            return false;
        }

        // DLL内のコードが利用できる間に旧オブジェクトを片付ける
        sceneRequest_->ClearGame();
        ShutdownGame();
        Unload();

        if (!Load(dllPath))
        {
            return false;
        }

        if (!InitializeGame())
        {
            sceneRequest_->ClearGame();
            ShutdownGame();
            Unload();
            return false;
        }

        return true;
    }
}