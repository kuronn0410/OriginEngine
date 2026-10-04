#include "GameMain.h"
#include "include/Scenes/SceneChangeRequest.h"  
#include "include/Scene/GameSceneType.h"
#include "include/Scene/GameSceneFactory.h"
#include <memory>
#include <utility>
#include "include/Scene/GameResources.h"

#include "include/Renderer/Resource/ResourceManager.h"
#include "include/Renderer/Model/ObjLoader.h"

bool GameMain::Initialize(SceneChangeRequest& sceneRequest, ResourceManager& resourceManager)
{
    /*---Meshの初期化---*/
	ObjLoader objLoader;
    meshData1_ = objLoader.Load("Assets/Resource/Try.obj");
	meshData2_ = objLoader.Load("Assets/Resource/Cube.obj");
    meshData3_ = objLoader.Load("Assets/Resource/Try1.obj");
    gameResources_.playerMesh = resourceManager.RegisterMesh(meshData3_);
    gameResources_.enemyMesh = resourceManager.RegisterMesh(meshData1_);
    gameResources_.CubeMesh = resourceManager.RegisterMesh(meshData2_);


    /*--Textureの初期化---*/
	gameResources_.backgroundTexture = resourceManager.RegisterTexture("Assets/Resource/Try.png");
	gameResources_.baseTexture = resourceManager.RegisterTexture("Assets/Resource/Try1.png");
	gameResources_.groundTexture = resourceManager.RegisterTexture("Assets/Resource/ground.png");
    /*-----------------*/

    sceneFactory_ = std::make_unique<GameSceneFactory>(sceneRequest,&gameResources_);

    sceneRequest.SetSceneFactory(std::move(sceneFactory_));

    sceneRequest.RequestInitialScene(GameSceneType::Test);
    
	return true;
}

void GameMain::Finalize()
{

}

