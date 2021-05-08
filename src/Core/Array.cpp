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

UnitTest(Array_Basic)
{
	{
		TArray<int> intArray;
		tcheck(intArray.GetCount() == 0);
		tcheck(intArray.GetData() == nullptr);
		tcheck(intArray.GetReservation() == 0);

		intArray.Add(5);
		tcheck(intArray.GetCount() == 1);
		tcheck(intArray.GetData() != nullptr);
		tcheck(intArray.GetReservation() == 0);
		tcheck(intArray[0] == 5);
		tcheck(&intArray[0] == intArray.GetData());

		intArray.Clear();
		tcheck(intArray.GetCount() == 0);
		tcheck(intArray.GetData() == nullptr);
		tcheck(intArray.GetReservation() == 0);
	}

	{
		TArray<int> intArray{1, 2, 3, 4, 5};
		tcheck(intArray.GetCount() == 5);
		tcheck(intArray.GetData() != nullptr);
		tcheck(intArray.GetReservation() == 0);

		int i = 1;
		for (int v : intArray)
		{
			tcheck(v == i);
			++i;
		}
		tcheck(i == 6);
	}
}

UnitTest(Array_ObjectLifetime)
{
	static uint constructCount = 0;
	static uint destructCount = 0;

	struct SConstructorMock
	{
		SConstructorMock()
		{
			++constructCount;
		}

		SConstructorMock(const SConstructorMock&)
		{
			++constructCount;
		}

		~SConstructorMock()
		{
			++destructCount;
		}
	};
	{
		TArray<SConstructorMock> mockArray;
		mockArray.Resize(511);
		tcheck(constructCount == 512); // 511 array elements + initial exemplar in Resize
		tcheck(destructCount == 1); // exemplar destroyed
	}
	tcheck(destructCount == 512); // array out of scope
	{
		constructCount = 0;
		destructCount = 0;
		TArray<SConstructorMock> mockArray;
		mockArray.Resize(511);
		tcheck(constructCount == 512); // 511 array elements + initial exemplar in Resize
		tcheck(destructCount == 1); // exemplar destroyed
		mockArray.Clear();
		tcheck(destructCount == 512); // array cleared
	}
}

UnitTest(Array_Memory)
{
	{
		TArray<int, TAllocatorMock<int>> intArray;
		TAllocatorMock<int>& allocator = intArray.GetAllocator();
		tcheck(allocator.TotalCount == 0);
		tcheck(allocator.AllocCount == 0);
		tcheck(allocator.ReAllocCount == 0);
		tcheck(allocator.FreeCount == 0);

		intArray.Reserve(50);
		tcheck(allocator.TotalCount == 1);
		tcheck(allocator.AllocCount == 1);
		tcheck(allocator.ReAllocCount == 0);
		tcheck(allocator.FreeCount == 0);

		intArray.Resize(50); // exactly the reservation: no need for allocation
		tcheck(allocator.TotalCount == 1);
		tcheck(allocator.AllocCount == 1);
		tcheck(allocator.ReAllocCount == 0);
		tcheck(allocator.FreeCount == 0);

		intArray.Add(999);
		tcheck(allocator.TotalCount == 2);
		tcheck(allocator.AllocCount == 1);
		tcheck(allocator.ReAllocCount == 1);
		tcheck(allocator.FreeCount == 0);

		intArray.Clear();
		tcheck(allocator.TotalCount == 3);
		tcheck(allocator.AllocCount == 1);
		tcheck(allocator.ReAllocCount == 1);
		tcheck(allocator.FreeCount == 1);
	}
}
