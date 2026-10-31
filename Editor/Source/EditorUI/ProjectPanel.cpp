#include "Include/EditorUI/ProjectPanel.h"
#include "External/imgui/imgui.h"
#include <system_error>
#include <Windows.h>
namespace Origin::Editor
{
	void ProjectPanel::Draw()
	{

		ImGui::Begin("OriginEngine Editor## ProjectPanel");

		static int clickCount = 0;

		ImGui::InputText(
			"Open##Project",
			projectPathBuffer_.data(),
			projectPathBuffer_.size()
		);

		if (ImGui::Button("Open Game Project", ImVec2(200, 50)))
		{
				std::u8string utf8Path(
					reinterpret_cast<const char8_t*>(projectPathBuffer_.data())
				);

				std::filesystem::path directory(utf8Path);
				projectManager_.LoadProject(directory);
			
			++clickCount;
		}

		ImGui::Text("Click Count: %d", clickCount);

		ImGui::End();
	}
}