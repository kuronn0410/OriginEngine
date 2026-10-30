#include "Include/EditorUI/ProjectPanel.h"
#include "External/imgui/imgui.h"
namespace Origin::Editor
{
	void ProjectPanel::Draw()
	{

		ImGui::Begin("OriginEngine Editor## ProjectPanel");

		static int clickCount = 0;

		ImGui::InputText(
			"Open##Project",
			sceneName_,
			sizeof(sceneName_)
		);

		if (ImGui::Button("Open Game Project", ImVec2(200, 50)))
		{
			++clickCount;
		}

		ImGui::Text("Click Count: %d", clickCount);

		ImGui::End();
	}
}