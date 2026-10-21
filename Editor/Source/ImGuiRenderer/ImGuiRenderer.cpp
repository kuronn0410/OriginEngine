#include "Include/ImGuiRenderer/ImGuiRenderer.h"
#include "External/imgui/imgui.h"
#include "External/imgui/backends/imgui_impl_win32.h"
#include "External/imgui/backends/imgui_impl_dx12.h"
#include <d3d12.h>
#include <Windows.h>
#include <dxgiformat.h>


bool ImGuiRenderer::Initialize(
    ID3D12Device* device,
    ID3D12CommandQueue* commandQueue,
    //スワップチェイン側
    int numFramesInFlight,
    DXGI_FORMAT rtvFormat,
    //ウィンドウ側
    HWND hwnd
    // Descriptor管理機能など
)
{
    // 1. ImGuiのバージョン確認
    IM_ASSERT(ImGui::GetCurrentContext() == NULL);
   // 2. ImGui Context生成
    ImGui::CreateContext();

   // 3. Win32バックエンド初期化
   if(!ImGui_ImplWin32_Init(hwnd))
   {
       ImGui::DestroyContext();
       return false;
   }

   // 4. 構造体ImGui_ImplDX12_InitInfoの初期化
   ImGui_ImplDX12_InitInfo init_info = {};
   init_info.Device = device;
   init_info.CommandQueue = commandQueue;
   init_info.NumFramesInFlight = numFramesInFlight;
   init_info.RTVFormat = rtvFormat; 
   init_info.SrvDescriptorHeap = &srvHeap_;
   init_info.UserData = &descriptorAllocator_;
   init_info.SrvDescriptorAllocFn =[](
           ImGui_ImplDX12_InitInfo* info,
           D3D12_CPU_DESCRIPTOR_HANDLE* outCpu,
           D3D12_GPU_DESCRIPTOR_HANDLE* outGpu
       )
       {
           auto* allocator =
               static_cast<DescriptorAllocator*>(info->UserData);

           auto allocation = allocator->Allocate();

           // 確保失敗時の処理
           if (!allocation.IsValid())
           {
               IM_ASSERT(false && "SRV descriptor allocation failed");
               *outCpu = {};
               *outGpu = {};
               return;
           }


           *outCpu = allocation.cpuHandle;
           *outGpu = allocation.gpuHandle;
       };
   init_info.SrvDescriptorFreeFn = [](
	   ImGui_ImplDX12_InitInfo* info,
	   D3D12_CPU_DESCRIPTOR_HANDLE cpu,
	   D3D12_GPU_DESCRIPTOR_HANDLE gpu
	   )
	   {
		   auto* allocator =
			   static_cast<DescriptorAllocator*>(info->UserData);
           // CPUハンドルからDescriptorを解放
		    allocator->Free(cpu);
	   };



   // 5. DX12初期化情報の設定
   if(!ImGui_ImplDX12_Init(&init_info))
   {
       
       ImGui_ImplWin32_Shutdown();
       ImGui::DestroyContext();
       return false;
   }
   // 8. 成功・失敗の判定
   return true;
}

void ImGuiRenderer::BeginFrame()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiRenderer::Render(
    ID3D12GraphicsCommandList& commandList
)
{
    // 1. ImGuiの描画データを生成
    ImGui::Render();

    // 2. 描画データをDirectX12のCommandListに記録
    ImGui_ImplDX12_RenderDrawData(
        ImGui::GetDrawData(),
        &commandList
    );
}

void ImGuiRenderer::Shutdown()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}