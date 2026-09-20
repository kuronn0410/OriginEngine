#include "Include/Window/Window.h"

/*
WinMain
↓
WNDCLASSを作る
↓
RegisterClassで登録
↓
CreateWindow
↓
ShowWindow
↓
GetMessageで待つ
↓
WindowProcで処理
↓
×ボタン
↓
WM_DESTROY
↓
PostQuitMessage
↓
終了
*/

//WNDCLASS windowClass = {};
bool Window::Initialize(HINSTANCE hInstance)
{
	WNDCLASS windowClass = {};

	windowClass.lpfnWndProc = WindowProc;// メッセージ処理をする関数
	windowClass.hInstance = hInstance;// このアプリのインスタンス
	windowClass.lpszClassName = L"MyWindow";

	if (!RegisterClass(&windowClass))// ウィンドウクラスの登録、メッセージの処理をする関数をOSに登録する
	{
		return false;
	}


	HWND createHwnd = CreateWindow(
		L"MyWindow",
		L"My Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
	 nullptr,
	 nullptr,
	 hInstance,
	 nullptr
	);

	if (!createHwnd)
	{
		return false;
	}
	this->hwnd = createHwnd;
	Show();
	return true;
}


//メッセージ	
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


void Window::Show()
{
	ShowWindow(this->hwnd, SW_SHOW);
}