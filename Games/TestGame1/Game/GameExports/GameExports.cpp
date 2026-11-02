#include "Include/GameModule/GameModuleAPI.h"

extern "C" __declspec(dllexport)
int GetGameVersion()
{
    return 1;
}