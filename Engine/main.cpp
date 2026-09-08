#include <Windows.h>
#include "Include/Window/Window.h"

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

	// メッセージループ用
    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        //TranslateMessage(&msg);
        DispatchMessage(&msg);//取得したメッセージを、そのウィンドウの WindowProc に送ります。
    }

    return 0;
}