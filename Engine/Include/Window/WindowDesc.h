#pragma once
#include <string>
struct WindowDesc
{
    std::wstring title = L"OriginEngine";
    int width = 1280;
    int height = 720;
    bool isMainWindow = false;
};