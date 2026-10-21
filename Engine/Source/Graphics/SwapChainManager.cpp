#include "Include/Graphics/SwapChainManager.h"
#include "Include/Graphics/SwapChainContext.h"
#include "Include/Window/WindowHandle.h"
#include <memory>
#include <Windows.h>
#include <utility>
#include <unordered_map>

bool SwapChainManager::Create(
    WindowHandle handle,
    HWND hwnd
)
{
    if (!handle.IsValid() || hwnd == nullptr)
    {
        return false;
    }
    if (contexts_.find(handle.id) != contexts_.end())
    {
        return false;
    }
	auto context = std::make_unique<SwapChainContext>();
	if(!context->Initialize(hwnd, factory_, commandQueue_,device_ ))
	{
		return false;
	}

	if(!contexts_.insert(std::make_pair(handle.id, std::move(context))).second)
	{
		return false;
	}

	return true;
};

DXGI_FORMAT SwapChainManager::GetRTVFormat(WindowHandle handle) 
{
    auto context = GetContext(handle);
    if (!context) {
        return DXGI_FORMAT_UNKNOWN;
    }
    return context->GetRTVFormat();
}

UINT SwapChainManager::GetBufferCount(WindowHandle handle)
{
    auto context = GetContext(handle);
    if (!context) {
        return 0;
    }
    return context->GetBufferCount();
}


SwapChainContext* SwapChainManager::GetContext(WindowHandle handle)
{
    auto it = contexts_.find(handle.id);
    if (it == contexts_.end()) {
        return nullptr;
    }
    return it->second.get();
}



bool SwapChainManager::Destroy(WindowHandle handle)
{
    return contexts_.erase(handle.id) > 0;
}

bool SwapChainManager::DestroyAll()
{
    contexts_.clear();
    return true;
}