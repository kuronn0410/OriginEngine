#include "Include/Graphics/OffscreenRenderTargetManager.h"
#include "Include/Graphics/DescriptorAllocation.h"
#include "Include/Graphics/DescriptorAllocator.h"
#include "Include/Graphics/OffscreenRenderTarget.h"
#include "Include/Graphics/OffscreenHandle.h"
#include <memory>
namespace Origin
{

	const OffscreenHandle OffscreenRenderTargetManager::CreateOffscreenRenderTarget(
		UINT width,
		UINT height
	)
	{
		auto offscreenRenderTarget = std::make_unique<OffscreenRenderTarget>();
		DescriptorAllocation srvAllocation = descriptorAllocator_.Allocate();
		if (srvAllocation.index == UINT32_MAX)
		{
			return OffscreenHandle{ UINT32_MAX };
		}
		if (!offscreenRenderTarget->Initialize(
			device_,
			width,
			height,
			srvAllocation
		))
		{
			descriptorAllocator_.Free(srvAllocation.index);
			return OffscreenHandle{ UINT32_MAX };
		}
		offscreenRenderTargets_.push_back(std::move(offscreenRenderTarget));
		return OffscreenHandle{ static_cast<uint32_t>(offscreenRenderTargets_.size() - 1) };

	}
	//破棄
	bool OffscreenRenderTargetManager::Destory(const OffscreenHandle& handle)
	{
		if (handle.id >= offscreenRenderTargets_.size())
		{
			return false;
		}
		auto& offscreenRenderTarget = offscreenRenderTargets_[handle.id];
		if (!offscreenRenderTarget)
		{
			return false;
		}
		descriptorAllocator_.Free(offscreenRenderTarget->GetSRVAllocation().index);
		offscreenRenderTarget.reset();
		return true;
	}
	void OffscreenRenderTargetManager::Clear()
	{
		for (uint32_t i = 0;
			i < offscreenRenderTargets_.size();
			++i)
		{
			// ① スロットにRenderTargetが存在するか確認
			if (!offscreenRenderTargets_[i])
			{
				continue;
			}
			// ② OffscreenHandleを作成
			OffscreenHandle handle{ i };

			// ③ Destroy(handle)を呼び出す
			Destory(handle);	
		}
	}
}