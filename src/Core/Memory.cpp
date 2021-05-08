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

#include "pch.h"
#include "Memory.h"

#include <mimalloc.h>

#ifdef PF_ENABLE_PROFILING
static struct
{
	size_t PurposeMemorySize[(uint)EAllocationPurpose::Max]{};
} gMemoryProfilingData;
#endif

void* FMemory::Alloc(const size_t size, const size_t alignment, const EAllocationPurpose purpose)
{
#ifdef PF_ENABLE_PROFILING
	gMemoryProfilingData.PurposeMemorySize[(uint)purpose] += mi_good_size(size);
#endif
	return mi_malloc_aligned(size, alignment);
}

void* FMemory::ReAlloc(void* initialMemory, const size_t size, const size_t alignment, const EAllocationPurpose purpose)
{
#ifdef PF_ENABLE_PROFILING
	gMemoryProfilingData.PurposeMemorySize[(uint)purpose] -= mi_usable_size(initialMemory);
	gMemoryProfilingData.PurposeMemorySize[(uint)purpose] += mi_good_size(size);
#endif
	return mi_realloc_aligned(initialMemory, size, alignment);
}

void FMemory::Free(void* memory, EAllocationPurpose purpose)
{
#ifdef PF_ENABLE_PROFILING
	gMemoryProfilingData.PurposeMemorySize[(uint)purpose] -= mi_usable_size(memory);
#endif
	mi_free(memory);
}

size_t FMemory::GetPurposeMemory(EAllocationPurpose purpose)
{
#ifdef PF_ENABLE_PROFILING
	return gMemoryProfilingData.PurposeMemorySize[(uint)purpose];
#else
	return 0;
#endif
}

void* operator new(const size_t size)
{
	return FMemory::Alloc(size);
}

void* operator new[](const size_t size)
{
	return FMemory::Alloc(size);
}

void operator delete(void* p) noexcept
{
	return FMemory::Free(p);
}

void operator delete[](void* p) noexcept
{
	return FMemory::Free(p);
}
