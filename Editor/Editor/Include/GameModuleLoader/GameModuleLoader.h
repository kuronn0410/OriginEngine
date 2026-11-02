#pragma once
#include <Windows.h>
#include <filesystem>
class GameModuleLoader
{
public:
    bool Load(const std::filesystem::path& dllpath);
    void Unload();

    ~GameModuleLoader();

private:
    HMODULE module_ = nullptr;
};