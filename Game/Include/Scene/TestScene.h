#pragma once
#include "include/Scenes/Scene.h"
#include "include/Renderer/Mesh.h"
#include "include/Renderer/Vertex.h"
#include "include/Renderer/MeshData.h" 
#include "include/Renderer/Object.h"

class SceneChangeRequest;
class Renderer;
class Graphics;
class GameResources;


class TestScene : public Scene
{
public:
    TestScene(SceneChangeRequest& sceneRequest,GameResources* gameResources):
        Scene(sceneRequest),
        gameResources_(gameResources)
    {   
    }
    bool Initialize(Graphics& graphics);
    void Update() override;
    void Render(Renderer& renderer) override;
private:
	//Mesh mesh_;
	//Mesh mesh1_;

 //   MeshData meshData_;
 //   MeshData meshData1_;
	Object object1_;
	Object object2_;
	GameResources* gameResources_ = nullptr;
};