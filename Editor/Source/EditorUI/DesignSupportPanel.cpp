#include "Include/EditorUI/DesignSupportPanel.h"
#include "External/imgui/imgui.h"

namespace Origin::Editor
{
	void DesignSupportPanel::Draw()
	{
		// Implement the drawing logic for the design support panel here

		ImGui::SetNextWindowPos(
			ImVec2(20, 20),
			ImGuiCond_Always
		);

		ImGui::SetNextWindowSize(
			ImVec2(350, 150),
			ImGuiCond_Always
		);
		ImGui::Begin("OriginEngine Editor");

		static int clickCount = 0;

		ImGui::InputText(
			"Scene",
			sceneName_,
			sizeof(sceneName_)
		);

		if (ImGui::Button("Generate Scene Code", ImVec2(200, 50)))
		{
			++clickCount;
			sceneCodeGenerator_.SceneCodeGenerate(sceneName_);
		}

		ImGui::Text("Click Count: %d", clickCount);

		ImGui::End();
	}
}