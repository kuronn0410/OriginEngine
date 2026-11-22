#include "Include/EditorUI/EditorPanelManager.h"

#include "Include/GameProject/ProjectManager.h"
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_internal.h"
#include "Include/GameModuleLoader/GameModuleLoader.h"
/*===パネル=====*/
#include "Include/EditorUI/GameViewPanel.h"
#include "Include/EditorUI/SceneViewPanel.h"
#include "Include/EditorUI/DesignSupportPanel.h"
#include "Include/EditorUI/ProjectPanel.h"
/*=============*/
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
		for (size_t i = 0; i < panels_.size(); ++i)
		{
			panels_[i]->Initialize();
		}
		AddDrawPanel(0);
		AddDrawPanel(1);


		return true;
	}

	bool EditorPanelManager::AddGamePanel(GameContext& gameContext, Graphics& graphics)
	{
		panels_.push_back(std::make_unique<GameViewPanel>(gameContext, graphics));
		panels_.push_back(std::make_unique<SceneViewPanel>(gameContext, graphics));
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
		AddDrawPanel(panels_.size() - 2);
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
		ImGuiID dockspaceId = ImGui::GetID("MainDockSpace");
		if(!isDefaultLayoutSetup_)
		{
			SetupDefaultLayout(dockspaceId);
			isDefaultLayoutSetup_ = true;
		}
		ImGui::DockSpaceOverViewport(dockspaceId);
		for(const auto& index : drawIndices_ )
		{
			panels_[index]->Draw();
		}
	}

	void EditorPanelManager::DrawMenuBar()
	{
	   
	}
	
	void EditorPanelManager::SetupDefaultLayout(ImGuiID dockspaceId)
	{
		// ① DockSpaceのサイズを設定
		ImGui::DockBuilderRemoveNode(dockspaceId);

		ImGui::DockBuilderAddNode(
			dockspaceId,
			ImGuiDockNodeFlags_DockSpace
		);

		ImGui::DockBuilderSetNodeSize(
			dockspaceId,
			ImGui::GetMainViewport()->Size
		);
		// ② 全体を左右に分割（左45%、右55%）
		ImGuiID rightNodeId;
		ImGuiID leftNodeId = ImGui::DockBuilderSplitNode(
			dockspaceId,
			ImGuiDir_Left,
			0.45f,
			nullptr,
			&rightNodeId
		);
		// ③ 左側を上下に分割（上50%、下50%）
		ImGuiID bottomNodeId;

		ImGuiID topNodeId = ImGui::DockBuilderSplitNode(
			leftNodeId,
			ImGuiDir_Up,
			0.5f,
			nullptr,
			&bottomNodeId
		);
		// ④ Game Viewを左上へ
		ImGui::DockBuilderDockWindow(
			"Game View",
			topNodeId
		);
		// ⑤ 3D Viewを左下へ
		ImGui::DockBuilderDockWindow(
			"Scene View",
			bottomNodeId
		);

		// ⑥ 設計サポートとInspectorを右側へ
		ImGui::DockBuilderDockWindow(
			"Design Support",
			rightNodeId
		);
		ImGui::DockBuilderDockWindow(
			"ProjectPanel",
			rightNodeId
		);	

		// ⑦ DockBuilderFinish()で確定
		ImGui::DockBuilderFinish(dockspaceId);
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