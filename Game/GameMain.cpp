#include "GameMain.h"
#include "include/Scene/TestScene.h"
#include "include/Scenes/SceneChangeRequest.h"  


bool GameMain::Initialize(SceneChangeRequest& sceneRequest)
{
    sceneFactory_ = std::make_unique<GameSceneFactory>(sceneRequest);

    sceneRequest.RequestInitialScene(
        std::make_unique<TestScene>(sceneRequest)
    );

    
	return true;
}

void GameMain::Finalize()
{

}

