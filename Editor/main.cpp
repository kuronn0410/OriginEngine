#include <Windows.h>
#include "Include/Graphics/Graphics.h"
#include "Include/Window/Window.h"
#include "Include/Scenes/SceneManager.h"
#include "include/Scenes/SceneChangeRequest.h"
#include "include/Renderer/Render/Renderer.h"
#include "include/Renderer/Render/ShaderCompiler.h"
#include "include/Graphics/Color.h"
//#include "GameMain.h"
#include "Include/Renderer/Resource/ResourceManager.h"
#include "Include/Window/WindowManager.h"
#include "Include/Window/WindowDesc.h"
#include "Include/Window/WindowHandle.h"
#include "Include/Graphics/SwapChainManager.h"
#include "Include/EditorUI/GameContext.h"
#include "Include/Editor.h"
#include "Include/GameModuleLoader/GameModuleLoader.h"
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

	Origin::WindowManager windowManager;
	

	Origin::WindowHandle mainEditorWindowHandle
		= windowManager.Create(hInstance, Origin::WindowDesc{ L"Editor Window", CW_USEDEFAULT, CW_USEDEFAULT,1600, 940, true, {} });
    Origin::WindowHandle editorWindowHandle
		= windowManager.Create(hInstance, Origin::WindowDesc{ L"Editor Window", 840, 20,730, 860, false, mainEditorWindowHandle });
    Origin::WindowHandle gameWindowHandle
        = windowManager.Create(hInstance, Origin::WindowDesc{ L"Game Window", 20, 20 ,800, 420, false, mainEditorWindowHandle });
    Origin::WindowHandle view3DWindowHandle
        = windowManager.Create(hInstance, Origin::WindowDesc{ L"3D View Window", 20, 460, 800, 420,false, mainEditorWindowHandle });

    
    //RECT rect{};
    //GetClientRect(windowManager.GetHwnd(editorWindowHandle), &rect);

    Origin::Graphics graphics;
    if (!graphics.Initialize(windowManager.GetHwnd(gameWindowHandle)))
    {
        return -1;
    }

	

    Origin::SwapChainManager swapChainManager(
		*graphics.GetFactory(),
		*graphics.GetDevice(),
		*graphics.GetCommandQueue()
	);

	if (!swapChainManager.Create(gameWindowHandle, windowManager.GetHwnd(gameWindowHandle)))
	{
		return -1;
	}

    if (!swapChainManager.Create(mainEditorWindowHandle, windowManager.GetHwnd(mainEditorWindowHandle)))
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
    
    //Origin::ShaderCompiler shaderCompiler;
    //if (!shaderCompiler.Initialize())
    //{
    //    return -1;
    //}
    //Origin::Renderer renderer;
    //if(!renderer.Initialize(shaderCompiler, *graphics.GetDevice()))
    //{
    //    return -1;
    //}
    //Origin::SceneManager sceneManager(graphics);
    

    Origin::Editor::Editor editor(graphics, windowManager);
    if(!editor.Initialize(
		swapChainManager.GetBufferCount(editorWindowHandle),
        swapChainManager.GetRTVFormat(editorWindowHandle),
        editorWindowHandle
    ))
    {
        return -1;
    }

    if (!graphics.BeginCommands())
    {
        return -1;
    }

    Origin::ShaderCompiler shaderCompiler;
	if (!shaderCompiler.Initialize())
	{
		return -1;
	}
	Origin::Renderer renderer;
    if (!renderer.Initialize(shaderCompiler, *graphics.GetDevice()))
    {
		return -1;
    }
    
    

    Origin::ResourceManager resourceManager(
        *graphics.GetDevice(),
		*graphics.GetCommandList(),
		*graphics.GetDescriptorAllocator()
    );

    Origin::SceneManager sceneManager(graphics);

    Origin::Editor::GameContext gameContext;

    gameContext.renderer = &renderer;
    gameContext.sceneManager = &sceneManager;
	editor.AddGamePanel(gameContext);

    Origin::SceneChangeRequest sceneRequest(sceneManager);

	editor.GameInitialize(sceneRequest, resourceManager);

    renderer.SetResourceManager(resourceManager);

    if (!graphics.EndCommands())
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
			graphics.BeginCommands();
            graphics.BeginWindowRender(Color(0.0f, 0.8f, 1.0f, 1.0f), windowManager.GetHwnd(gameWindowHandle), *swapChainManager.GetContext(gameWindowHandle));
            renderer.BeginFrame(graphics.GetCommandList(), graphics.GetSRVHeap());
            sceneManager.Render(renderer);
            graphics.EndWindowRender(*swapChainManager.GetContext(gameWindowHandle));
			graphics.EndCommands();
            graphics.Present(*swapChainManager.GetContext(gameWindowHandle));
        }

        if (windowManager.Exists(mainEditorWindowHandle))
        {
            sceneManager.Update();
            graphics.BeginCommands();
            graphics.BeginWindowRender(Color(0.65f, 0.65f, 0.65f, 1.0f), windowManager.GetHwnd(mainEditorWindowHandle), *swapChainManager.GetContext(mainEditorWindowHandle));
            renderer.BeginFrame(graphics.GetCommandList(), graphics.GetSRVHeap());
            graphics.EndWindowRender(*swapChainManager.GetContext(mainEditorWindowHandle));
			graphics.EndCommands();
            graphics.Present(*swapChainManager.GetContext(mainEditorWindowHandle));
            
        }
        if (windowManager.IsExitRequested())
        {
            break;
        }

        // 前フレームのボタン操作を、描画開始前に処理する
        editor.Update();

        if (windowManager.Exists(editorWindowHandle))
        {
            graphics.BeginCommands();
            graphics.BeginWindowRender(Color(1.0f, 0.0f, 0.0f, 1.0f), windowManager.GetHwnd(editorWindowHandle), *swapChainManager.GetContext(editorWindowHandle));
            editor.Draw();
            graphics.EndWindowRender(*swapChainManager.GetContext(editorWindowHandle));
			graphics.EndCommands();
            graphics.Present(*swapChainManager.GetContext(editorWindowHandle));
        }

        if (windowManager.Exists(view3DWindowHandle))
        {
            sceneManager.Update();
            graphics.BeginCommands();
            graphics.BeginWindowRender(Color(0.0f, 0.4f, 0.8f, 1.0f), windowManager.GetHwnd(view3DWindowHandle), *swapChainManager.GetContext(view3DWindowHandle));
            renderer.BeginFrame(graphics.GetCommandList(), graphics.GetSRVHeap());
            sceneManager.Render(renderer);
            graphics.EndWindowRender(*swapChainManager.GetContext(view3DWindowHandle));
            graphics.EndCommands();
            graphics.Present(*swapChainManager.GetContext(view3DWindowHandle));
            

        }
        
            //graphics.Render();
        
    }
   

    windowManager.DestroyAll();

    return 0;
}