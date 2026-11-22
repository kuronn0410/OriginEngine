#include "Include/EditorUI/EditorPanelManager.h"
#include "Include/EditorUI/DesignSupportPanel.h"
#include "Include/EditorUI/ProjectPanel.h"
#include "Include/GameProject/ProjectManager.h"
#include "External/imgui/imgui.h"
#include "Include/GameModuleLoader/GameModuleLoader.h"
#include "Include/EditorUI/GameViewPanel.h"
#include "Include/EditorUI/GameContext.h"
#include "Include/Graphics/Graphics.h"
#include "Include/Graphics/OffscreenRenderTargetManager.h"
#include "Include/EditorUI/IOffscreenRenderable.h"
#include <memory>
#include <algorithm>
namespace Origin::Editor
{

	bool EditorPanelManager::Initialize(
		ProjectManager& projectManager, 
		GameModuleLoader& gameModuleLoader
	)
	{
		panels_.push_back(std::make_unique<DesignSupportPanel>(projectManager));
		panels_.push_back(std::make_unique<ProjectPanel>(projectManager, gameModuleLoader));
		//panels_.push_back(std::make_unique<GameViewPanel>(graphics, offscreenRenderTargetManager));
		for (size_t i = 0; i < panels_.size(); ++i)
		{
			panels_[i]->Initialize();
			//auto* renderable =
			//	dynamic_cast<IOffscreenRenderable*>(panels_[i].get());

			//if (renderable)
			//{
			//	offscreenPanels_.push_back(renderable);
			//}
		}
		AddDrawPanel(0);
		AddDrawPanel(1);


		return true;
	}

	bool EditorPanelManager::AddGamePanel(GameContext& gameContext, Graphics& graphics)
	{
		panels_.push_back(std::make_unique<GameViewPanel>(gameContext, graphics));

		for (size_t i = 0; i < panels_.size(); ++i)
		{
			panels_[i]->Initialize();
			auto* renderable =
					dynamic_cast<IOffscreenRenderable*>(panels_[i].get());

			if (renderable)
			{
				offscreenPanels_.push_back(renderable);
			}
		}
		AddDrawPanel(panels_.size() - 1);

		return true;
	}
	void EditorPanelManager::RenderOffscreen()
	{
		for (auto* panel : offscreenPanels_)
		{
			panel->RenderOffscreen();
		}
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