#pragma once
#include <Windows.h>
#include "include/Window/WindowDesc.h"
#include "Include/Window/WindowEvent.h"
#include <queue>
#include <functional>
class Window
{
public:
	//bool Initialize(HINSTANCE hInstance);// ウィンドウの初期化
	bool Initialize(HINSTANCE hInstance, const WindowDesc& desc, HWND parentHwnd);
	void Show();// ウィンドウの表示	
	HWND GetHwnd() const { return hwnd_; }
	bool PollEvent(WindowEvent& event);
	bool Destroy();

	using MessageHandler =
		std::function<bool(HWND, UINT, WPARAM, LPARAM)>;

	void SetMessageHandler(MessageHandler handler)
	{
		messageHandler_ = std::move(handler);
	}

private:
	static LRESULT CALLBACK WindowProc(
		HWND hwnd, 
		UINT msg, 
		WPARAM wParam, 
		LPARAM lParam
	);
	HWND hwnd_;
	std::queue<WindowEvent> events_;
	MessageHandler messageHandler_;
};