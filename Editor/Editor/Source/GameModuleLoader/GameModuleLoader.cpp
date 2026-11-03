#include "Include/GameModuleLoader/GameModuleLoader.h"
#include "Include/Scenes/SceneChangeRequest.h"
#include "Include/Renderer/Resource/ResourceManager.h"

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



bool GameModuleLoader::InitializeGame(
    Origin::SceneChangeRequest& sceneRequest,
    Origin::ResourceManager& resourceManager)
{
    if (!module_)
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

    return initialize(sceneRequest, resourceManager);
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