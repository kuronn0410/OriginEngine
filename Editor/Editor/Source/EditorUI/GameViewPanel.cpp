#include "Include/EditorUI/GameViewPanel.h"
#include "External/imgui/imgui.h"
#include <d3d12.h>
#include <Windows.h>
#include <wrl/client.h>
namespace Origin::Editor
{
	bool GameViewPanel::Initialize()
	{
		offscreenHandle_ = gameContext_.offscreenRenderTargetManager->CreateOffscreenRenderTarget(800, 600);
		return true;
	}
	void GameViewPanel::RenderOffscreen()
	{
		// ① HandleからOffscreenRenderTargetを取得
		OffscreenRenderTarget* offscreenRenderTarget = gameContext_.offscreenRenderTargetManager->GetOffscreenRenderTarget(offscreenHandle_);
		// ② nullptrを確認
		if (!offscreenRenderTarget)
		{
			return;
		}
		gameContext_.sceneManager->Update();
		// ③ graphics_.BeginOffscreenRender()
		graphics_.BeginOffscreenRender(*offscreenRenderTarget);
		gameContext_.renderer->BeginFrame(graphics_.GetCommandList(), graphics_.GetSRVHeap());
		gameContext_.sceneManager->Render(*gameContext_.renderer);
		// ④ graphics_.EndOffscreenRender()
		graphics_.EndOffscreenRender(*offscreenRenderTarget);
	}
	void GameViewPanel::Draw()
	{
		ImGui::Begin("Game View");

		// ① HandleからRenderTargetを取得
		OffscreenRenderTarget* target = gameContext_.offscreenRenderTargetManager->GetOffscreenRenderTarget(offscreenHandle_);	
		if (target)
		{
			// ② SRVのGPU Descriptor Handleを取得
			D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = target->GetSRVAllocation().gpuHandle;
			// ③ ImGui::Image()で表示
			ImTextureID textureID =
				static_cast<ImTextureID>(srvHandle.ptr);
			ImGui::Image(textureID, ImVec2(800, 600));
		}
		
		
		ImGui::End();
	}
}