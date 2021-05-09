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
}