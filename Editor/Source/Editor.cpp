#include "Include/Editor.h"
#include "Include/Graphics/Graphics.h"
#include "Include/ImGuiRenderer/ImGuiRenderer.h"
#include "External/imgui/imgui.h"
#include "Include/Window/WindowHandle.h"
#include "Include/GameProject/ProjectManager.h"
#include <Windows.h>
#include <memory>
#include <dxgiformat.h>
#include "Include/EditorUI/DesignSupportPanel.h"
// 前方宣言
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

namespace Origin::Editor
{
	bool Editor::Initialize(
		int numFramesInFlight,
		DXGI_FORMAT rtvFormat,
		//ウィンドウ側
		WindowHandle windowHandle)
	{


		DesignSupportPanel designSupportPanel;
		panels_.push_back(std::make_unique<DesignSupportPanel>());

		windowHandle_ = windowHandle;
		// ImGuiRendererの初期化
		imguiRenderer_.Initialize(
			graphics_.GetDevice(),
			graphics_.GetCommandQueue(),
			numFramesInFlight,
			rtvFormat,
			windowManager_.GetHwnd(windowHandle_)
		);


		//メッセージハンドラを登録する
		if (!windowManager_.SetMessageHandler(
			windowHandle_,
			[](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)-> bool
			{
				if (msg == WM_LBUTTONDOWN)
				{
					SetFocus(hwnd);
				}
				ImGui_ImplWin32_WndProcHandler(
					hwnd, msg, wp, lp
				);
				return false;
			}
		))
		{
			imguiRenderer_.Shutdown();
			return false;
		}

		//テスト
		bool result = projectManager_.LoadProject(
			L"C:/YY/ゲーム制作/DirectX/OriginEngine/OriginEngine/Games/TestGame1"
		);


		if (!result)
		{
			// 読み込み失敗
			return false;
		}

		const GameProject* project =
			projectManager_.GetCurrentProject();

		if (project != nullptr)
		{
			// ブレークポイントを設定して内容を確認
		}




		return true;
	}
	void Editor::Update()
	{

	}
	void Editor::Draw()
	{
		imguiRenderer_.BeginFrame();
		// UIを構築
		panels_[0]->Draw();
		imguiRenderer_.Render(*graphics_.GetCommandList());
	}
	void Editor::Shutdown()
	{
		// 1. Windowの入力コールバックを解除
		windowManager_.SetMessageHandler(
			windowHandle_,
			{}
		);

		// 2. ImGuiの終了処理
		imguiRenderer_.Shutdown();
	}
}