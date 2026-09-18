#include "GameMain.h"

#include "include/Scene/TestScene.h"
#include "include/Scene/TitleScene.h"



bool GameMain::Initialize(SceneChangeRequest& sceneRequest)
{
    sceneRequest.RequestInitialScene(
        std::make_unique<TitleScene>()
    );

	return true;
}

void GameMain::Finalize()
{

}

