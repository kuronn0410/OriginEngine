#include "Include/Window/Window.h"
#include "Include/Window/WindowDesc.h"
#include "Include/Window/WindowEvent.h"
#include "Include/Window/WindowEventType.h"
#include <Windows.h>

bool Window::Initialize(
	HINSTANCE hInstance, 
	const WindowDesc& desc, 
	HWND parentHwnd)
{
	WNDCLASS windowClass = {};

	windowClass.lpfnWndProc = WindowProc;// メッセージ処理をする関数
	windowClass.hInstance = hInstance;// このアプリのインスタンス
	windowClass.lpszClassName = L"MyWindow";

	if (!RegisterClass(&windowClass))// ウィンドウクラスの登録、メッセージの処理をする関数をOSに登録する
	{
		DWORD error = GetLastError();
		if (error != ERROR_CLASS_ALREADY_EXISTS)
		{
			return false;
		}

	}

	DWORD style = parentHwnd != nullptr
		? WS_CHILD | WS_VISIBLE
		: WS_OVERLAPPEDWINDOW;

	HWND createHwnd = CreateWindow(
		L"MyWindow",
		desc.title.c_str(),
		style,
		desc.x,
		desc.y,
		desc.width,
		desc.height,
		parentHwnd,
		nullptr,
		hInstance,
		this  // Windowインスタンスへのポインター
	);

	if (!createHwnd)
	{
		return false;
	}
	this->hwnd_ = createHwnd;
	Show();
	return true;
}

bool Window::PollEvent(WindowEvent& event)
{
	// ① キューが空ならfalseを返す
	if (events_.empty())
	{
		return false;
	}
	// ② キューの先頭のイベントをeventにコピー
	event = events_.front();
	// ③ キューから先頭イベントを削除
	events_.pop();
	// ④ trueを返す
	return true;	
}

//メッセージ	
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		// ① lParamからCREATESTRUCTW*を取得
		auto* createStruct =
			reinterpret_cast<CREATESTRUCTW*>(lParam);
		// ② CREATESTRUCTWからWindow*を取得
		Window* window =
			static_cast<Window*>(createStruct->lpCreateParams);
		// ③ GWLP_USERDATAにWindow*を登録
		SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(window)
		);
		return TRUE;
	}

	Window* window = reinterpret_cast<Window*>(
		GetWindowLongPtrW(hwnd, GWLP_USERDATA)
		);

	// 登録されたコールバックを呼ぶ
	if (window && window->messageHandler_)
	{
		window->messageHandler_(
			hwnd, msg, wParam, lParam
		);
	}

	switch (msg)
	{
	case WM_CLOSE:
	{
		Window* window = reinterpret_cast<Window*>(
			GetWindowLongPtrW(hwnd, GWLP_USERDATA)
			);

		if (window == nullptr)
		{
			return DefWindowProcW(hwnd, msg, wParam, lParam);
		}

		WindowEvent event;
		event.type = WindowEventType::CloseRequested;

		window->events_.push(event);

		return 0;
	}
	case WM_DESTROY:
		// ① GWLP_USERDATAからWindow*を取得
		Window* window = reinterpret_cast<Window*>(
			GetWindowLongPtrW(hwnd, GWLP_USERDATA)
			);

		// ② Windowが保持するhwnd_をnullptrにする
		if (window)
		{
			window->hwnd_ = nullptr;
		}

		// ③ GWLP_USERDATAにnullptrを登録して関連付けを解除
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);

		// ④ DefWindowProcWに処理を渡す
		return DefWindowProcW(hwnd, msg, wParam, lParam);
		
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}


void Window::Show()
{
	ShowWindow(this->hwnd_, SW_SHOW);
}

bool Window::Destroy()
{
	if (hwnd_ == nullptr)
	{
		return false;
	}

	if (!::DestroyWindow(hwnd_))
	{
		return false;
	}

	hwnd_ = nullptr;
	return true;
}