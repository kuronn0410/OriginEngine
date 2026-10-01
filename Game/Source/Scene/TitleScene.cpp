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
    MeshHandle meshHandle1_ = gameResources_->playerMesh;
    object_.SetTransform({
        { 0.0f, -3.0f, 0.0f },//P
        { 0.0f, 0.0f, 0.0f },//R
        { 0.5f, 0.5f, 0.5f }//S
        });

    float z = object_.GetTransform().rotation.z;
    object_.SetMeshHandle(meshHandle1_);


    /*--------Cameraの初期化----------*/
    camera_.SetTransform({
        { 0.0f, 0.0f, -5.0f },
        { 0.0f, 0.0f, 0.0f }//縦、横
        });

    camera_.SetProjectionSettings({
        60.0f,//fov
        1280.0f / 720.0f,//aspectRatio
        0.1f,//nearPlane
        100.0f//farPlane
        });
	// 仮実装
	return true;
}

void TitleScene::Render(Renderer& renderer)
{
	renderer.SetCamera(camera_);
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