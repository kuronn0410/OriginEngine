#include "include/Scene/TitleScene.h"
#include "include/Graphics/Graphics.h"
#include "include/Scene/GameSceneType.h"
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Renderer/Render/Renderer.h"
#include <Windows.h>//GetAsyncKeyState
#include "include/Scene/GameResources.h"
#include "include/Renderer/Mesh/MeshHandle.h"

bool TitleScene::Initialize(Graphics& graphics)
{
    MeshHandle meshHandle1_ = gameResources_->enemyMesh;
    object_.SetTransform({ 0.0f, 0.0f, 0.0f });
    object_.SetMeshHandle(meshHandle1_);


	// 仮実装
	return true;
}

void TitleScene::Render(Renderer& renderer)
{
    renderer.Draw(object_);
}

void TitleScene::Update()
{
    // ここに更新処理を書く
    if (GetAsyncKeyState('2') & 0x8000)
    {
        sceneRequest_.RequestChangeScene(GameSceneType::Test);
    }
}