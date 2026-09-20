#include <Windows.h>
#include "Include/Graphics/Graphics.h"
#include "Include/Window/Window.h"
#include "Include/Scenes/SceneManager.h"
#include "include/Scenes/SceneChangeRequest.h"
#include "GameMain.h"


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

    SceneManager sceneManager;

    SceneChangeRequest sceneRequest(sceneManager);

    GameMain gameMain;
    if (!gameMain.Initialize(sceneRequest))
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
			sceneManager.Render(graphics);
            //graphics.Render();
        }
    }
   

    return 0;
}