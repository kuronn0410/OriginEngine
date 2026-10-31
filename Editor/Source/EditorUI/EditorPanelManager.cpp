#include "Include/EditorUI/EditorPanelManager.h"
#include "Include/EditorUI/DesignSupportPanel.h"
#include "Include/EditorUI/ProjectPanel.h"
#include "External/imgui/imgui.h"
#include <memory>
#include <algorithm>
namespace Origin::Editor
{

	bool EditorPanelManager::Initialize()
	{
		panels_.push_back(std::make_unique<DesignSupportPanel>());
		panels_.push_back(std::make_unique<ProjectPanel>());

		AddDrawPanel(0);
		AddDrawPanel(1);
		return true;
	}
	void EditorPanelManager::Draw()
	{
		ImGui::DockSpaceOverViewport();
		for(const auto& index : drawIndices_ )
		{
			panels_[index]->Draw();
		}
	}

	void EditorPanelManager::DrawMenuBar()
	{
	   
	}


	
	bool EditorPanelManager::AddDrawPanel(uint32_t index)
	{
		auto it = std::find(drawIndices_.begin(), drawIndices_.end(), index);
		
		if(it != drawIndices_.end())
		{
			
			return false;
		}
		drawIndices_.push_back(index);
		return true;
	}

	bool EditorPanelManager::RemoveDrawPanel(uint32_t index)
	{
		auto it = std::find(drawIndices_.begin(), drawIndices_.end(), index);

		if (it == drawIndices_.end())
		{

			return false;
		}
		drawIndices_.erase(it);
		return true;
	}



}