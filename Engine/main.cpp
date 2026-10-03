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
#include <sal.h>

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    Window window;
    if (!window.Initialize(hInstance))
    {
        return -1;
    }

    

    Graphics graphics;
    if (!graphics.Initialize(window.GetHwnd()))
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

    while (msg.message != WM_QUIT)
    {
        //TranslateMessage(&msg);
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);//取得したメッセージを、そのウィンドウの WindowProc に送ります。
        }
        else
        {
            sceneManager.Update();

			graphics.BeginFrame(Color(0.0f, 0.0f, 0.0f, 1.0f), window.GetHwnd());
            renderer.BeginFrame(graphics.GetCommandList());
			sceneManager.Render(renderer);
			graphics.EndFrame();
            //graphics.Render();
        }
    }
   

    return 0;
}