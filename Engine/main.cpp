#include <Windows.h>
#include "Include/Graphics/Graphics.h"
#include "Include/Window/Window.h"
#include "Include/Scenes/SceneManager.h"
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Renderer/Render/Renderer.h"
#include "include/Renderer/Render/ShaderCompiler.h"
#include "include/Graphics/Color.h"
#include "GameMain.h"
#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/Window/WindowManager.h"
#include "Include/Window/WindowDesc.h"
#include "Include/Window/WindowHandle.h"
#include "Include/Graphics/SwapChainManager.h"
#include "Include/Editor.h"
#include <sal.h>
#include <cstdint>

#include "Include/Graphics/DescriptorAllocation.h"
#include "Include/Graphics/DescriptorAllocator.h"
#include <cassert>

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{


    /*-----------------テスト-----------------------*/
    //D3D12_CPU_DESCRIPTOR_HANDLE cpuStart{};
    //D3D12_GPU_DESCRIPTOR_HANDLE gpuStart{};

    //DescriptorAllocator allocator(
    //    cpuStart,
    //    gpuStart,
    //    1,  // Descriptorサイズ（テスト用）
    //    3   // 最大3個
    //);

    //auto a = allocator.Allocate(); // index 0
    //auto b = allocator.Allocate(); // index 1
    //auto c = allocator.Allocate(); // index 2
    //auto d = allocator.Allocate(); // 無効

    //allocator.Free(b.index);

    //auto e = allocator.Allocate(); // index 1を再利用
    //assert(a.index == 0);
    //assert(b.index == 1);
    //assert(c.index == 2);
    //assert(!d.IsValid());
    //assert(e.index == 1);
    //allocator.Free(1);
    //allocator.Free(1);  // 二重解放

    //allocator.Free(100); // 範囲外

    //auto f = allocator.Allocate();
    //auto g = allocator.Allocate();

    
    /*----------------------------------------------------*/


	WindowManager windowManager;
	

	WindowHandle editorWindowHandle
		= windowManager.Create(hInstance, WindowDesc{ L"Editor Window", CW_USEDEFAULT, CW_USEDEFAULT,1280, 720, true, {} });
    WindowHandle gameWindowHandle
        = windowManager.Create(hInstance, WindowDesc{ L"Game Window", 0 , 0 ,640, 350, false, editorWindowHandle });
    WindowHandle view3DWindowHandle
        = windowManager.Create(hInstance, WindowDesc{ L"3D View Window", 0 , 360 , 640, 350,false, editorWindowHandle });


    Graphics graphics;
    if (!graphics.Initialize(windowManager.GetHwnd(gameWindowHandle)))
    {
        return -1;
    }

	Editor editor(graphics);

	SwapChainManager swapChainManager(
		*graphics.GetFactory(),
		*graphics.GetDevice(),
		*graphics.GetCommandQueue()
	);

	if (!swapChainManager.Create(gameWindowHandle, windowManager.GetHwnd(gameWindowHandle)))
	{
		return -1;
	}

    if (!swapChainManager.Create(editorWindowHandle, windowManager.GetHwnd(editorWindowHandle)))
    {
        return -1;
    }
	if(!swapChainManager.Create(view3DWindowHandle, windowManager.GetHwnd(view3DWindowHandle)))
	{
		return -1;
	}


    if (!graphics.BeginInitializationCommands())
    {
        return -1;
    }

    ShaderCompiler shaderCompiler;
	if (!shaderCompiler.Initialize())
	{
		return -1;
	}
	Renderer renderer;
    if (!renderer.Initialize(shaderCompiler, *graphics.GetDevice()))
    {
		return -1;
    }
    ResourceManager resourceManager(
        *graphics.GetDevice(),
		*graphics.GetCommandList(),
		*graphics.GetSRVHeap(),
		graphics.GetSRVDescriptorSize(),
		graphics.GetMaxSRVCount()
    );

    SceneManager sceneManager(graphics);

    SceneChangeRequest sceneRequest(sceneManager);

    GameMain gameMain;
    if (!gameMain.Initialize(sceneRequest, resourceManager))
    {
        return -1;
    }

    renderer.SetResourceManager(resourceManager);
    if (!graphics.EndInitializationCommands())
    {
        return -1;
    }
	// メッセージループ用
    MSG msg = {};

    while (!windowManager.IsExitRequested())
    {

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // ② WindowManagerのイベントを処理
        windowManager.ProcessEvents();

        // ③ 終了要求の確認
        if (windowManager.IsExitRequested())
        {
            break;
        }

        //TranslateMessage(&msg);
        if(windowManager.Exists(gameWindowHandle))
        {
            sceneManager.Update();
            graphics.BeginFrame(Color(0.0f, 0.8f, 1.0f, 1.0f), windowManager.GetHwnd(gameWindowHandle), *swapChainManager.GetContext(gameWindowHandle));
            renderer.BeginFrame(graphics.GetCommandList());
            sceneManager.Render(renderer);
            graphics.EndFrame(*swapChainManager.GetContext(gameWindowHandle));
        }

        if (windowManager.Exists(editorWindowHandle))
        {
            sceneManager.Update();
            graphics.BeginFrame(Color(0.0f, 0.0f, 0.0f, 1.0f), windowManager.GetHwnd(editorWindowHandle), *swapChainManager.GetContext(editorWindowHandle));
            renderer.BeginFrame(graphics.GetCommandList());
            //sceneManager.Render(renderer);
            graphics.EndFrame(*swapChainManager.GetContext(editorWindowHandle));
            
        }

        if (windowManager.Exists(view3DWindowHandle))
        {
            sceneManager.Update();
            graphics.BeginFrame(Color(0.0f, 0.4f, 0.8f, 1.0f), windowManager.GetHwnd(view3DWindowHandle), *swapChainManager.GetContext(view3DWindowHandle));
            renderer.BeginFrame(graphics.GetCommandList());
            sceneManager.Render(renderer);
            graphics.EndFrame(*swapChainManager.GetContext(view3DWindowHandle));

        }
        
            //graphics.Render();
        
        



    }
   

    windowManager.DestroyAll();

    return 0;
}