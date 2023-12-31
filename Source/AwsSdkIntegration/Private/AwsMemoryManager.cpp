// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#include "AwsMemoryManager.h"

void FUnrealMemoryManager::Begin()
{
}

void FUnrealMemoryManager::End()
{
}

void* FUnrealMemoryManager::AllocateMemory(std::size_t blockSize, std::size_t alignment, const char* allocationTag)
{
	return FMemory::Malloc(blockSize, alignment);
}

void FUnrealMemoryManager::FreeMemory(void* memoryPtr)
{
	FMemory::Free(memoryPtr);
}
