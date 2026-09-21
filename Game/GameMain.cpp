#include "GameMain.h"
#include "include/Scenes/SceneChangeRequest.h"  
#include "include/Scene/GameSceneType.h"
#include "include/Scene/GameSceneFactory.h"
#include <memory>
#include <utility>



bool GameMain::Initialize(SceneChangeRequest& sceneRequest)
{
    sceneFactory_ = std::make_unique<GameSceneFactory>(sceneRequest);

    sceneRequest.SetSceneFactory(std::move(sceneFactory_));

    sceneRequest.RequestInitialScene(GameSceneType::Test);
    
	return true;
}

void GameMain::Finalize()
{

}

