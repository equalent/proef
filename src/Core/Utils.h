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

namespace FUtils
{
	template<typename T>
	struct Less
	{
		constexpr bool operator()(const T& v1, const T& v2)
		{
			return v1 < v2;
		}
	};
	
	template<typename T>
	void Swap(T& v1, T& v2)
	{
		const T initial = v1;
		v1 = v2;
		v2 = initial;
	}
}

/* Reverse iterators wrapper for range-based for (https://stackoverflow.com/a/28139075) */
template <typename T>
struct TReverseWrapper { T& Iterable; };

template <typename T>
auto begin(TReverseWrapper<T> wrapper) { return std::rbegin(wrapper.Iterable); }

template <typename T>
auto end(TReverseWrapper<T> wrapper) { return std::rend(wrapper.Iterable); }

template <typename T>
TReverseWrapper<T> reverse(T&& iterable) { return { iterable }; }
