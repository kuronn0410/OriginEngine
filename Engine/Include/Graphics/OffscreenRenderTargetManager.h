#pragma once
#include "Include/Graphics/OffscreenRenderTarget.h"
#include "Include/Graphics/OffscreenHandle.h"
#include <vector>
#include <memory>
namespace Origin
{
	class OffscreenRenderTargetManager
	{
	public:
		OffscreenRenderTargetManager(DescriptorAllocator& descriptorAllocator, ID3D12Device& device) : 
			descriptorAllocator_(descriptorAllocator), device_(device) {}
		const OffscreenHandle CreateOffscreenRenderTarget(
			UINT width,
			UINT height
		);
		OffscreenRenderTarget* GetOffscreenRenderTarget(const OffscreenHandle& handle)
		{
			if (handle.id < offscreenRenderTargets_.size())
			{
				return offscreenRenderTargets_[handle.id].get();
			}
			return nullptr;
		}
		//破棄
		bool Destory(const OffscreenHandle& handle);
		void Clear();
		~OffscreenRenderTargetManager() {
			Clear();
		}
	private:
		DescriptorAllocator& descriptorAllocator_;
		ID3D12Device& device_;
		// SRVの割り当て情報
		std::vector<std::unique_ptr<OffscreenRenderTarget>> offscreenRenderTargets_;
	};

}