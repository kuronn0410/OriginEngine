#include "Include/Editor.h"
#include "Include/Graphics/Graphics.h"
#include "Include/ImGuiRenderer/ImGuiRenderer.h"
#include "External/imgui/imgui.h"

bool Editor::Initialize()
{
	//imguiRenderer_.Initialize(
	//	graphics_.GetDevice(),
	//	graphics_.GetCommandQueue(),
	//	graphics_.GetNumFramesInFlight(),
	//	graphics_.GetRtvFormat(),
	//	graphics_.GetHwnd()
	//);
	return true;
}
void Editor::Update()
{

}
void Editor::Draw()
{
	imguiRenderer_.BeginFrame();
	// UIを構築
	ImGui::Begin("OriginEngine Editor");

	ImGui::Text("Hello, OriginEngine!");
	ImGui::Button("Test Button");

	ImGui::End();
	imguiRenderer_.Render(*graphics_.GetCommandList());
}
void Editor::Shutdown()
{

}