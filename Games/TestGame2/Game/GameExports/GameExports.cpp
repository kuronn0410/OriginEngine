#include "Include/GameModule/GameModuleAPI.h"
#include "GameMain.h"
#include <memory>
#include <utility>
extern "C" __declspec(dllexport)
int GetGameVersion()
{
    return 1;
}


namespace
{
    // DLL側が所有し、初期化後も保持する
    std::unique_ptr<Game::GameMain> gameMain;
}
extern "C" __declspec(dllexport)
bool InitializeGame(
    Origin::SceneChangeRequest& sceneRequest,
    Origin::ResourceManager& resourceManager
)
{

    if (gameMain)
    {
        return false; // 初期化済み
    }

    auto instance = std::make_unique<Game::GameMain>();

    if (!instance->Initialize(sceneRequest, resourceManager))
    {
        return false;
    }

    gameMain = std::move(instance);
    return true;
}

extern "C" __declspec(dllexport)
void ShutdownGame()
{
    if (gameMain)
    {
        gameMain->Finalize();
        gameMain.reset();
    }
}