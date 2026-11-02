#include "Include/GameModuleLoader/GameModuleLoader.h"

bool GameModuleLoader::Load(const std::filesystem::path& dllpath)
{
	//もし既にロード済みのDLLがある場合はfalseを返す
    if (module_ != nullptr)
    {
		return false;
    }
    //bool exists = std::filesystem::exists(dllpath);
    //std::filesystem::path absolutePath =
    //    std::filesystem::absolute(dllpath);

    //OutputDebugStringW(L"DLL Path: ");
    //OutputDebugStringW(absolutePath.c_str());
    //OutputDebugStringW(L"\n");
    module_ = LoadLibraryW(dllpath.c_str());

    if (module_ == nullptr)
    {
        DWORD error = GetLastError(); // ブレークポイント
        return false;
    }
    FARPROC address = GetProcAddress(
        module_,
        "GetGameVersion"
    );
    if (!address)
    {
        FreeLibrary(module_);
        module_ = nullptr;
        return false;
    }
    using GetGameVersionFunc = int (*)();
    GetGameVersionFunc getVersion =
        reinterpret_cast<GetGameVersionFunc>(address);

    int version = getVersion();
    return true;
}

void GameModuleLoader::Unload()
{
    if (module_)
    {
        FreeLibrary(module_);
        module_ = nullptr;
    }
}

GameModuleLoader::~GameModuleLoader()
{
    Unload();
}