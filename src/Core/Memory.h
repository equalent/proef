/*
 * Proef
 *
 * Copyright (c) Andrey Tsurkan
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

enum class EAllocationPurpose : uint
{
	General,
	InternalString,

	Max
};

struct FMemory
{
	static void* Alloc(size_t size, size_t alignment = 1, EAllocationPurpose purpose = EAllocationPurpose::General);
	static void* ReAlloc(void* initialMemory, size_t size, size_t alignment = 1, EAllocationPurpose purpose = EAllocationPurpose::General);
	static void Free(void* memory, EAllocationPurpose purpose = EAllocationPurpose::General);
	static size_t GetPurposeMemory(EAllocationPurpose purpose);
	
	FORCEINLINE static void Copy(void* dst, void const* src, const size_t size)
	{
		memcpy(dst, src, size);
	}
};

template<typename T, EAllocationPurpose TPurpose = EAllocationPurpose::General, size_t TSize = sizeof(T)>
struct TRawAllocator
{
	/**
	 * This allocator can handle allocations of N > 1. When this value is false, N must be 1 and ReAlloc must never be called
	 */
	const static bool kCanAllocateMany = true;

	FORCEINLINE TRawAllocator() = default;
	FORCEINLINE TRawAllocator(const TRawAllocator& other) = default; // no need to copy anything, it's a static allocator
	FORCEINLINE TRawAllocator(TRawAllocator&& other) = default; // no need to move anything, it's a static allocator
	
	FORCEINLINE static T* Alloc(const size_t n, const size_t alignment = 1)
	{
		return (T*)FMemory::Alloc(TSize * n, alignment, TPurpose);
	}

	FORCEINLINE static T* ReAlloc(T* obj, const size_t n, const size_t alignment = 1)
	{
		return (T*)FMemory::ReAlloc(obj, TSize * n, alignment, TPurpose);
	}

	FORCEINLINE static void Free(T* obj)
	{
		FMemory::Free(obj, TPurpose);
	}
};

#ifdef PF_UNIT_TEST
template<typename T, typename TAllocator = TRawAllocator<T>>
struct TAllocatorMock
{
	const static bool kCanAllocateMany = TAllocator::kCanAllocateMany;
	
	TAllocator Allocator;
	
	size_t AllocCount = 0;
	size_t ReAllocCount = 0;
	size_t FreeCount = 0;
	size_t TotalCount = 0;

	FORCEINLINE T* Alloc(const size_t n, const size_t alignment = 1)
	{
		++AllocCount;
		++TotalCount;
		return Allocator.Alloc(n, alignment);
	}

	FORCEINLINE T* ReAlloc(T* obj, const size_t n, const size_t alignment = 1)
	{
		++ReAllocCount;
		++TotalCount;
		return Allocator.ReAlloc(obj, n, alignment);
	}

	FORCEINLINE void Free(T* obj)
	{
		++FreeCount;
		++TotalCount;
		Allocator.Free(obj);
	}
};
#endif
