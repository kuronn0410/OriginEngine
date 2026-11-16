#include "Include/EditorUI/GameViewPanel.h"
#include "External/imgui/imgui.h"
namespace Origin::Editor
{
	bool GameViewPanel::Initialize()
	{
		return true;
	}
	void GameViewPanel::Draw()
	{
		ImGui::Begin("Game View");


		ImGui::Text("Game View Panel");
		ImGui::End();
	}
}