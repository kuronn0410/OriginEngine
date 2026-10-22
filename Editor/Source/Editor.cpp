#include "Include/Editor.h"
#include "Include/Graphics/Graphics.h"
#include "Include/ImGuiRenderer/ImGuiRenderer.h"
#include "External/imgui/imgui.h"

#include <Windows.h>
#include "External/imgui/backends/imgui_impl_win32.h"
// 前方宣言
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);


bool Editor::Initialize(
	int numFramesInFlight,
	DXGI_FORMAT rtvFormat,
	//ウィンドウ側
	WindowHandle windowHandle)
{
	
	// ImGuiRendererの初期化
	imguiRenderer_.Initialize(
		graphics_.GetDevice(),
		graphics_.GetCommandQueue(),
		numFramesInFlight,
		rtvFormat,
		windowManager_.GetHwnd(windowHandle)
	);


	//メッセージハンドラを登録する
	if (!windowManager_.SetMessageHandler(
		windowHandle,
		[](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)-> bool
		{
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
	return true;
}
void Editor::Update()
{

}
void Editor::Draw()
{
	imguiRenderer_.BeginFrame();
	// UIを構築
	ImGui::SetNextWindowPos(
		ImVec2(20, 20),
		ImGuiCond_Always
	);

	ImGui::SetNextWindowSize(
		ImVec2(500, 400),
		ImGuiCond_Always
	);
	ImGui::Begin("OriginEngine Editor");

	static int clickCount = 0;

	if (ImGui::Button("Test Button", ImVec2(200, 50)))
	{
		++clickCount;
	}

	ImGui::Text("Click Count: %d", clickCount);

	ImGui::End();
	imguiRenderer_.Render(*graphics_.GetCommandList());
}
void Editor::Shutdown()
{

}