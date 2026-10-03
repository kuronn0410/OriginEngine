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
	meshData3_ = objLoader.Load("Assets/Resource/Try1.obj");

    meshData1_ = objLoader.Load("Assets/Resource/Try.obj");
    meshData2_.vertices = {
        { { -0.5f,  0.5f, 0.0f } }, // 0 左上
        { {  0.5f,  0.5f, 0.0f } }, // 1 右上
        { {  0.5f, -0.5f, 0.0f } }, // 2 右下
        { { -0.5f, -0.5f, 0.0f } }  // 3 左下
    };// 三角形の頂点座標

    meshData2_.indices = {
        0, 1, 2,
        0, 2, 3
    };

	gameResources_.backgroundTexture = resourceManager.RegisterTexture("Assets/Resource/Try.png");
	gameResources_.playerMesh = resourceManager.RegisterMesh(meshData3_);
    gameResources_.enemyMesh = resourceManager.RegisterMesh(meshData1_);
    gameResources_.treeMesh = resourceManager.RegisterMesh(meshData2_);
    /*-----------------*/

    sceneFactory_ = std::make_unique<GameSceneFactory>(sceneRequest,&gameResources_);

    sceneRequest.SetSceneFactory(std::move(sceneFactory_));

    sceneRequest.RequestInitialScene(GameSceneType::Test);
    
	return true;
}

void GameMain::Finalize()
{

}

