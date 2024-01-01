// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#pragma once

THIRD_PARTY_INCLUDES_START
#include "aws/core/utils/memory/MemorySystemInterface.h"
THIRD_PARTY_INCLUDES_END

/**
 * Allows the aws-sdk-cpp to use Unreal's FMemory system as a memory allocator.
 * This is crucial to avoid memory crashes caused by allocator mismatch on platforms like iOS
 * @note This requires the aws-sdk-cpp to be built using USE_AWS_MEMORY_MANAGEMENT
 */
class FUnrealMemoryManager : public Aws::Utils::Memory::MemorySystemInterface
{
	// Begin MemorySystemInterface
	virtual void Begin() override;
	virtual void End() override;
	virtual void* AllocateMemory(std::size_t blockSize, std::size_t alignment, const char* allocationTag) override;
	virtual void FreeMemory(void* memoryPtr) override;
	// End MemorySystemInterface
};
