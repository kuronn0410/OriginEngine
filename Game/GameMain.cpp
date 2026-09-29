#include "GameMain.h"
#include "include/Scenes/SceneChangeRequest.h"  
#include "include/Scene/GameSceneType.h"
#include "include/Scene/GameSceneFactory.h"
#include <memory>
#include <utility>
#include "include/Scene/GameResources.h"

#include "include/Renderer/ResourceManager.h"


bool GameMain::Initialize(SceneChangeRequest& sceneRequest, ResourceManager& resourceManager)
{
    /*---Meshの初期化---*/
    
    meshData1_.vertices = {
    	{ 0.0f, 0.5f, 0.0f },   // 上の頂点
    	{ 0.5f, -0.5f, 0.0f },  // 右下の頂点
    	{ -0.5f, -0.5f, 0.0f }  // 左下の頂点
    };// 三角形の頂点座標
    meshData2_.vertices = {
        { 0.0f,  0.8f, 0.0f },   // 上
        { 0.8f, -0.3f, 0.0f },   // 右下
        {-0.3f, -0.6f, 0.0f }    // 左下
    };// 三角形の頂点座標

    gameResources_.playerMesh = resourceManager.RegisterMesh(meshData1_);
    gameResources_.enemyMesh = resourceManager.RegisterMesh(meshData2_);
    /*-----------------*/

    sceneFactory_ = std::make_unique<GameSceneFactory>(sceneRequest,&gameResources_);

    sceneRequest.SetSceneFactory(std::move(sceneFactory_));

    sceneRequest.RequestInitialScene(GameSceneType::Test);
    
	return true;
}

void GameMain::Finalize()
{

}

