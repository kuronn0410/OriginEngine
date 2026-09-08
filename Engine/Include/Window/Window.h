#pragma once
#include <Windows.h>
class Window
{
public:
	bool Initialize(HINSTANCE hInstance);// ウィンドウの初期化
	void Show();// ウィンドウの表示	
private:
	static LRESULT CALLBACK WindowProc(
		HWND hwnd, 
		UINT msg, 
		WPARAM wParam, 
		LPARAM lParam
	);
	HWND hwnd;

};