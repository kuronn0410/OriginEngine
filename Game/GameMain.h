#pragma once
#include "include/Scenes/SceneChangeRequest.h"

class GameMain
{
public:
	bool Initialize(SceneChangeRequest& sceneRequest);
	void Finalize();
};