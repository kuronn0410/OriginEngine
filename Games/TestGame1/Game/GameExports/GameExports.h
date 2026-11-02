#pragma once

extern "C"
{
    using GetGameVersionFunc = int (*)();

    __declspec(dllexport)
        int GetGameVersion();
}