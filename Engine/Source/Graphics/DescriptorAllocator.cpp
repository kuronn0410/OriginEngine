#include "Include/Graphics/DescriptorAllocation.h"
#include "Include/Graphics/DescriptorAllocator.h"
#include <cstdint>

DescriptorAllocation DescriptorAllocator::Allocate()
{
	uint32_t index;//今回割り当てる番号

	if(freeIndices_.size() > 0)
	{
		index = freeIndices_.back();
		freeIndices_.pop_back();
	}
	else if(nextIndex_ >= capacity_)
	{
		return DescriptorAllocation{};
	}
	else
	{
		index = nextIndex_;
		nextIndex_ += 1;
	}

	DescriptorAllocation allocation{};
	allocation.index = index;
	allocation.cpuHandle = cpuStart_;
	allocation.cpuHandle.ptr += 
		static_cast<SIZE_T>(descriptorSize_)*index;

	allocation.gpuHandle = gpuStart_;
	allocation.gpuHandle.ptr +=static_cast<UINT64>(descriptorSize_)*index;

	
	return allocation;
}

void DescriptorAllocator::Free(uint32_t index)
{
	if(index >= capacity_)
	{
		return;
	}

	if (index >= nextIndex_)
	{
		return;
	}

	auto it = std::find(freeIndices_.begin(), freeIndices_.end(), index);

	if(it != freeIndices_.end())
	{
		return;
	}

	

	freeIndices_.push_back(index);
}