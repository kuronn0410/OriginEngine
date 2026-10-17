#pragma once
#include <vector>
#include <memory>
#include <Windows.h>
#include "Include/Window/Window.h"
#include "Include/Window/WindowHandle.h"
#include "Include/Window/WindowDesc.h"


class WindowManager
{
public:
    WindowHandle Create(HINSTANCE hInstance, const WindowDesc& desc);
    HWND GetHwnd(WindowHandle handle) const;
    void ProcessEvents();
    bool IsExitRequested() const{ return exitRequested_; }
    bool Exists(WindowHandle handle) const;
    void DestroyAll();
private:
    bool DestroyWindow(WindowHandle handle);
    std::vector<std::unique_ptr<Window>> windows_;
    WindowHandle mainWindowHandle_;
    bool exitRequested_ = false;
};