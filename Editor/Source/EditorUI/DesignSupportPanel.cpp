#include "Include/EditorUI/DesignSupportPanel.h"
#include "External/imgui/imgui.h"

namespace Origin::Editor
{
	void DesignSupportPanel::Draw()
	{
		// Implement the drawing logic for the design support panel here

		ImGui::SetNextWindowPos(
			ImVec2(20, 20),
			ImGuiCond_Once
		);

		ImGui::SetNextWindowSize(
			ImVec2(500, 400),
			ImGuiCond_Once
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