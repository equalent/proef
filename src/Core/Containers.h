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

template<typename TFirst, typename TSecond>
struct TPair
{
	using FirstType = TFirst;
	using SecondType = TSecond;
	
	TFirst First;
	TSecond Second;

	TPair(const TFirst& first, const TSecond& second) : First(first), Second(second)
	{
	}
};

template<typename TFirst, typename TSecond, typename TCompare = FUtils::Less<TFirst>>
struct TPairFirstCompare
{
	using PairType = TPair<TFirst, TSecond>;
	
	TCompare Compare;
	
	constexpr bool operator()(const PairType& v1, const PairType& v2)
	{
		return Compare(v1.First, v2.First);
	}

	// overloads that allow TMap to find nodes by key only (without the whole pair)
	constexpr bool operator()(const TFirst& v1, const PairType& v2)
	{
		return Compare(v1, v2.First);
	}

	constexpr bool operator()(const PairType& v1, const TFirst& v2)
	{
		return Compare(v1.First, v2);
	}
};