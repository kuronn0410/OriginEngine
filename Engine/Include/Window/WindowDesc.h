#pragma once
#include <string>
#include "Include/Window/WindowHandle.h" 

namespace Origin
{
    struct WindowDesc
    {
        std::wstring title = L"OriginEngine";
        int x = CW_USEDEFAULT;
        int y = CW_USEDEFAULT;
        int width = 1280;
        int height = 720;
        bool isMainWindow = false;

        WindowHandle parent;
    };
}