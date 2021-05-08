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

/**
 * A UTF-8 string
 */
class FString
{
	using CharType = char;
	TArray<CharType, TRawAllocator<CharType, EAllocationPurpose::InternalString>> m_Data;

public:
	FORCEINLINE FString() : m_Data({0})
	{
	}

	FORCEINLINE FString(const CharType* rawString)
	{
		const size_t strLen = strlen(rawString);
		m_Data.Resize(strLen + 1);
		FMemory::Copy(m_Data.GetData(), rawString, sizeof(CharType) * strLen);
	}

	FORCEINLINE size_t GetLength() const
	{
		return m_Data.GetCount() - 1;
	}

	FORCEINLINE char& operator[](const size_t index)
	{
		return m_Data[index];
	}

	FORCEINLINE char operator[](const size_t index) const
	// char instead of char& to avoid additional memory address overhead
	{
		return m_Data[index];
	}

	FORCEINLINE char* GetData()
	{
		return m_Data.GetData();
	}

	FORCEINLINE const char* GetData() const
	{
		return m_Data.GetData();
	}

	FORCEINLINE void Resize(const size_t len)
	{
		m_Data.Resize(len + 1, 0);
		m_Data[len] = 0; // ensure safe memory access
	}

	FORCEINLINE FString& operator+=(const FString& other)
	{
		const size_t initialLen = GetLength();
		Resize(GetLength() + other.GetLength());
		FMemory::Copy(GetData() + initialLen, other.GetData(), other.GetLength());
		return *this;
	}

	/**
	 * Repeat string N times
	 */
	FORCEINLINE FString& operator*=(const uint n)
	{
		const size_t initialLen = GetLength();
		const size_t finalLen = initialLen * n;
		
		Resize(finalLen);

		char* const pBegin = GetData() + initialLen;
		for (char* p = pBegin; p < pBegin + finalLen; p += initialLen)
		{
			FMemory::Copy(p, m_Data.GetData(), initialLen);
		}
		m_Data[initialLen * n] = 0;

		return *this;
	}

	template<typename...Args>
	static FString PrintF(const FString& format, Args...args)
	{
		char buffer[1024];
		sprintf(buffer, format.GetData(), args...);

		return FString(buffer);
	}
};
