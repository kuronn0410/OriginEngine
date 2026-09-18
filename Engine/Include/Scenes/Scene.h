#pragma once
#include "include/Graphics/Color.h"
#include "include/Graphics/Graphics.h"



class Scene
{
public:
	virtual ~Scene() = default;// 仮想デストラクタを定義しておくことで、派生クラスのデストラクタが正しく呼ばれるようにする
	virtual void Update() = 0;
	virtual void Render(Graphics& graphics) = 0;

};