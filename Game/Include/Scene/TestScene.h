#pragma once
#include "include/Scenes/Scene.h"
#include "include/Renderer/Mesh.h"
#include "include/Renderer/Vertex.h"

class SceneChangeRequest;
class Renderer;
class Graphics;

class TestScene : public Scene
{
public:
    TestScene(SceneChangeRequest& sceneRequest):
        Scene(sceneRequest)
    {   
    }
    bool Initialize(Graphics& graphics);
    void Update() override;
    void Render(Renderer& renderer) override;
private:
	Mesh mesh_;
	Mesh mesh1_;
	Vertex vertices_[3] = {
        { 0.0f,  0.25f, 0.0f },  // 上
        { 0.8f, -0.25f, 0.0f },  // 右下
        {-0.8f, -0.25f, 0.0f }   // 左下
	};// 三角形の頂点座標
    Vertex vertices1_[3] = {
       { 0.0f, -0.45f, 0.0f },  // 上
       { 0.2f, -0.75f, 0.0f },  // 右下
       {-0.2f, -0.75f, 0.0f }   // 左下
    };// 三角形の頂点座標

};