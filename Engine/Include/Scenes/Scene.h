#pragma once
//class Graphics;
class Renderer;
namespace Origin
{
	class SceneChangeRequest;
}

class Graphics;

namespace Origin
{
	class Scene
	{
	public:
		Scene(Origin::SceneChangeRequest& sceneRequest) :
			sceneRequest_(sceneRequest)
		{
		}
		virtual ~Scene() = default;// 仮想デストラクタを定義しておくことで、派生クラスのデストラクタが正しく呼ばれるようにする
		virtual bool Initialize(Graphics& graphics) = 0;
		virtual void Update() = 0;
		virtual void Render(Renderer& renderer) = 0;
	protected:
		Origin::SceneChangeRequest& sceneRequest_;
	};
}