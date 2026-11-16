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

		//gameContext_.sceneManager->Update();
		//graphics_.BeginFrame(Color(0.0f, 0.4f, 0.8f, 1.0f), windowManager_.GetHwnd(windowHandle_), *graphics_.GetContext(windowHandle_));	
		//gameContext_.renderer->BeginFrame(graphics_.GetCommandList(), graphics_.GetSRVHeap());
		//gameContext_.sceneManager->Render(*gameContext_.renderer);
		//graphics_.EndFrame();
		ImGui::Text("Game View Panel");
		ImGui::End();
	}
}