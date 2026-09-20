#pragma once
class Graphics;
class SceneChangeRequest;

class Scene
{
public:
	Scene(SceneChangeRequest& sceneRequest) :
		sceneRequest_(sceneRequest)
	{
	}
	virtual ~Scene() = default;// 仮想デストラクタを定義しておくことで、派生クラスのデストラクタが正しく呼ばれるようにする
	virtual void Update() = 0;
	virtual void Render(Graphics& graphics) = 0;
protected:
	SceneChangeRequest& sceneRequest_;
};