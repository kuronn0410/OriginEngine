#include "Include/Window/WindowManager.h"
#include "Include/Window/Window.h"
#include "Include/Window/WindowDesc.h"
#include "Include/Window/WindowHandle.h"
#include <vector>
#include <memory>
#include <Windows.h>
#include <cstdint>
#include <utility>

WindowHandle WindowManager::Create(HINSTANCE hInstance, const WindowDesc& desc)
{
	 auto window = std::make_unique<Window>();
	 if (!window->Initialize(hInstance, desc))
	 {
		 // 登録失敗
		 return WindowHandle{};
	 }

	 windows_.push_back(std::move(window));
	 WindowHandle handle{ static_cast<uint32_t>(windows_.size() - 1) };

	 if (desc.isMainWindow)
	 {
		 mainWindowHandle_ = handle;
	 }

	 return handle;
}



void WindowManager::ProcessEvents()
{
	std::vector<WindowHandle> destroyTargets;
	for (size_t i = 0; i < windows_.size(); ++i)
	{
		auto& window = windows_[i];

		// ① nullptrならスキップ
		if(!window)
		{
			continue;
		}
		// ② WindowEventを用意
		WindowEvent event;
		WindowHandle handle{ static_cast<uint32_t>(i) };
		
		
		// ③ PollEvent()でイベントを取り出す
		while(window->PollEvent(event))
		{
			// ④ CloseRequestedなら破棄対象として記録
			if (event.type == WindowEventType::CloseRequested)
			{
				// 破棄対象として記録
				if(handle.id == mainWindowHandle_.id)
				{
					// メインウィンドウが閉じられた場合、アプリケーションを終了
					exitRequested_ = true;
				}
				else
				{
					destroyTargets.push_back(handle);
				}
			}
		}
	}
	
	// 破棄対象のウィンドウを処理
	for (const auto& handle : destroyTargets)
	{
		if(!DestroyWindow(handle))
		{
			// 失敗した場合はエラー処理
			return;
		}
	}
}

bool WindowManager::DestroyWindow(WindowHandle handle)
{
	if (handle.id >= windows_.size() ||
		!windows_[handle.id])
	{
		return false;
	}
	if(!windows_[handle.id]->Destroy())
	{
		return false;
	}

	windows_[handle.id].reset();
	return true;
}

void WindowManager::DestroyAll()
{
	for (auto& window : windows_)
	{
		if(!window)
		{
			continue;
		}

		if (!window->Destroy())
		{
			// 失敗した場合はエラー処理
			continue;
		}
		window.reset();
	}
}

bool WindowManager::Exists(WindowHandle handle) const
{
	return windows_[handle.id] != nullptr;
}

HWND WindowManager::GetHwnd(WindowHandle handle) const
{
	return windows_[handle.id]->GetHwnd();
}