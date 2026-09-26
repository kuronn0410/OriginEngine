#pragma once
#include "include/Scenes/Scene.h"
#include "include/Renderer/Mesh.h"
#include "include/Renderer/Vertex.h"
class SceneChangeRequest;
class Renderer;
class Graphics;

class TitleScene : public Scene
{
public:
    TitleScene(SceneChangeRequest& sceneRequest) :
        Scene(sceneRequest)
    {
    }
    bool Initialize(Graphics& graphics);
    void Update() override;
    void Render(Renderer& renderer) override;
private:
	Mesh mesh_;
	Vertex vertices_[3] = {
		{ 0.0f, 0.5f, 0.0f },   // 上の頂点
		{ 0.5f, -0.5f, 0.0f },  // 右下の頂点
		{ -0.5f, -0.5f, 0.0f }  // 左下の頂点
	};// 三角形の頂点座標
};