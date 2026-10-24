#pragma once
#include "Include/Graphics/DescriptorAllocation.h"
#include <cstdint>
#include <d3d12.h>
#include <Windows.h>
#include <vector>
namespace Origin
{
	class DescriptorAllocator
	{
	public:
		DescriptorAllocator(
			D3D12_CPU_DESCRIPTOR_HANDLE cpuStart, 
			D3D12_GPU_DESCRIPTOR_HANDLE gpuStart, 
			UINT descriptorSize, 
			UINT capacity
		) :
			cpuStart_{cpuStart},
			gpuStart_{gpuStart},
			descriptorSize_(descriptorSize),
			capacity_(capacity)
		{
		}
		DescriptorAllocation Allocate();

		void Free(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle);
		void Free(uint32_t index);
	private:
		D3D12_CPU_DESCRIPTOR_HANDLE cpuStart_{};
		D3D12_GPU_DESCRIPTOR_HANDLE gpuStart_{};

		UINT descriptorSize_ = 0;
		UINT capacity_ = 0;
		uint32_t nextIndex_ = 0;//次に新規割り当てする番号
		std::vector<uint32_t> freeIndices_ = {};//解放された番号のリスト
	};
}