#include "Include/EditorUI/DesignSupportPanel.h"
#include "External/imgui/imgui.h"

void DesignSupportPanel::Draw()
{
	// Implement the drawing logic for the design support panel here

	ImGui::SetNextWindowPos(
		ImVec2(20, 20),
		ImGuiCond_Always
	);

	ImGui::SetNextWindowSize(
		ImVec2(500, 400),
		ImGuiCond_Always
	);
	ImGui::Begin("OriginEngine Editor");

	static int clickCount = 0;

	if (ImGui::Button("Test Button", ImVec2(200, 50)))
	{
		++clickCount;
		sceneCodeGenerator_.SceneCodeGenerate("Test");
	}

	ImGui::Text("Click Count: %d", clickCount);

	ImGui::End();
}