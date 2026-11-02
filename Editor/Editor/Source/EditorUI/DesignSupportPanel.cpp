#include "Include/EditorUI/DesignSupportPanel.h"
#include "External/imgui/imgui.h"

namespace Origin::Editor
{
	void DesignSupportPanel::Draw()
	{
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
			
			sceneCodeGenerator_.SceneCodeGenerate(sceneName_, *projectManager_.GetCurrentProject());
		}

		ImGui::Text("Click Count: %d", clickCount);

		ImGui::End();
	}
}