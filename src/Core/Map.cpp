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

UnitTest(Map_Basic)
{
	{
		TMap<int, int> intIntMap;
		intIntMap.Insert(3, 10);
		intIntMap.Insert(21, 10);
		intIntMap.Insert(32, 10);
		intIntMap.Insert(17, 199);
		intIntMap.Insert(7, 10444);
		tcheck(intIntMap.GetCount() == 5);
		tcheck(intIntMap[17] == 199);
		tcheck(intIntMap[7] == 10444);
		intIntMap[7] = 99;
		tcheck(intIntMap[7] == 99);

		int validKeys[] = { 3, 7, 17, 21, 32 }; // map keys sorted
		uint i = 0;
		for(TPair<int, int> pair : intIntMap)
		{
			tcheck(validKeys[i] == pair.First);
			++i;
		}
		tcheck(i == 5);

		i = 5;
		for (TPair<int, int> pair : reverse(intIntMap))
		{
			tcheck(validKeys[i - 1] == pair.First);
			--i;
		}
		tcheck(i == 0);

		tcheck(intIntMap.GetTree().GetBlackHeight() == 2);
		
		intIntMap.Clear();
		tcheck(intIntMap.GetCount() == 0);
	}

	{
		TMap<int, int> map;
		map.Insert(40, 10);
		map.Insert(55, 10);
		map.Insert(65, 10);
		map.Insert(60, 10);
		map.Insert(75, 10);
		map.Insert(57, 10);
		tcheck(map.GetCount() == 6);
		
		map.Remove(60);
		tcheck(map.GetCount() == 5);
	}
}

UnitTest(Map_ObjectLifetime)
{
	static uint constructCount = 0;
	static uint copyConstructCount = 0;
	static uint destructCount = 0;

	struct SConstructorMock
	{
		SConstructorMock()
		{
			++constructCount;
		}

		SConstructorMock(const SConstructorMock&)
		{
			++copyConstructCount;
		}

		~SConstructorMock()
		{
			++destructCount;
		}
	};

	TMap<int, SConstructorMock> map;
	tcheck(map.GetCount() == 0);
	tcheck(constructCount == 0);
	tcheck(destructCount == 0);

	map[5] = SConstructorMock();
	tcheck(map.GetCount() == 1);
	tcheck(constructCount == 2); // 1 in the test body, 1 in operator[] (initial value)
	tcheck(copyConstructCount == 2); // 1 when creating TPair, 1 when copying TPair to 
	tcheck(destructCount == 3); // destroyed instances: 1 in the test body, 1 in operator[] (initial value), 1 when creating TPair
	map.Clear();
	tcheck(destructCount == 4); // destroyed instance: actual data in the tree
}