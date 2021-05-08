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

template <typename T, typename TAllocator = TRawAllocator<T>>
class TArray
{
	static_assert(TAllocator::kCanAllocateMany, "TArray requires an allocator with kCanAllocateMany");

	TAllocator m_Allocator;

	/**
	 * The internal array pointer
	 */
	T* m_Array = nullptr;

	/**
	 * Number of elements in the array
	 */
	size_t m_Count = 0;

	/**
	 * Number of reserved element slots
	 */
	size_t m_Reservation = 0;

	/**
	 * Resize array memory directly
	 */
	FORCEINLINE void RawResize(const size_t newSize)
	{
		if (newSize == 0)
		{
			if (m_Array)
			{
				m_Allocator.Free(m_Array);
				m_Array = nullptr;
			}
		}
		else
		{
			if (m_Array)
			{
				m_Array = m_Allocator.ReAlloc(m_Array, newSize);
			}
			else
			{
				m_Array = m_Allocator.Alloc(newSize);
			}
		}
	}

	FORCEINLINE void InitFromRaw(const T* arr, const size_t count)
	{
		m_Count = count;
		RawResize(m_Count);

		for (size_t i = 0; i < count; ++i)
		{
			new(m_Array + i) T(arr[i]);
		}
	}

	FORCEINLINE void InternalShrink(const size_t count) // separate function to avoid unnecessary exemplar construction
	{
		check(count < m_Count);
		const size_t deletedElements = m_Count - count;
		for (size_t i = m_Count - 1; i < m_Count - 1 + deletedElements; ++i)
		{
			m_Array[i].~T(); // call destructors on all elements
		}
		m_Count = count;
		RawResize(m_Count + m_Reservation); // reallocate array with new count
	}

public:
	using AllocatorType = TAllocator;

	FORCEINLINE TArray() = default;

	explicit FORCEINLINE TArray(const size_t count, const T* data)
	{
		InitFromRaw(data, count);
	}

	FORCEINLINE TArray(const TArray& other)
	{
		InitFromRaw(other.m_Array, other.m_Count);
	}

	FORCEINLINE TArray(TArray&& other) noexcept
	{
		m_Allocator = std::move(other.m_Allocator);
		m_Count = std::move(other.m_Count);
		m_Array = std::move(other.m_Array);
	}

	FORCEINLINE TArray(std::initializer_list<T> initializerList)
	{
		InitFromRaw(initializerList.begin(), initializerList.size());
	}

	FORCEINLINE ~TArray()
	{
		Clear();
	}

	FORCEINLINE void Reserve(const size_t reservation)
	{
		if (reservation > m_Reservation)
		{
			m_Reservation = reservation;
			RawResize(m_Count + m_Reservation);
		}
	}

	FORCEINLINE void Clear()
	{
		if (!m_Count)return;

		InternalShrink(0);
	}

	FORCEINLINE bool IsEmpty() const
	{
		return m_Count == 0;
	}

	FORCEINLINE void Resize(const size_t count, const T& exemplar = T())
	{
		if (count < m_Count) // shrink array
		{
			InternalShrink(count);
		}
		else if (count > m_Count) // expand array
		{
			const size_t addedElements = count - m_Count;
			const size_t initialCount = m_Count;
			m_Count = count;
			if (m_Reservation >= addedElements) // if reservation can handle all added elements
			{
				m_Reservation -= addedElements; // place new elements in previously reserved memory
				//if(m_Reservation) // if there's reserved space left
				//{
				//	RawResize(m_Count + m_Reservation); // reallocate array to place elements that didn't get into pre-reserved memory
				//}
			}
			else // if reservation is smaller than added elements
			{
				m_Reservation = 0; // place all new elements in pre-reserved memory
				RawResize(m_Count + m_Reservation);
				// reallocate array to place elements that didn't get into pre-reserved memory 
			}

			for (size_t i = initialCount; i < initialCount + addedElements; ++i)
			{
				new(m_Array + i) T(exemplar); // call copy constructors (<-- exemplar) on all elements
			}
		}
	}

	FORCEINLINE void Add(const T& obj)
	{
		Resize(m_Count + 1, obj);
	}

	FORCEINLINE T& operator[](size_t index)
	{
		return m_Array[index];
	}

	FORCEINLINE const T& operator[](size_t index) const
	{
		return m_Array[index];
	}

	FORCEINLINE T* GetData()
	{
		return m_Array;
	}

	FORCEINLINE const T* GetData() const
	{
		return m_Array;
	}

	FORCEINLINE size_t GetCount() const
	{
		return m_Count;
	}

	FORCEINLINE size_t GetReservation() const
	{
		return m_Reservation;
	}

	FORCEINLINE TAllocator& GetAllocator()
	{
		return m_Allocator;
	}

	FORCEINLINE T* begin()
	{
		return m_Array;
	}

	FORCEINLINE const T* begin() const
	{
		return m_Array;
	}

	FORCEINLINE T* end()
	{
		return m_Array + m_Count;
	}

	FORCEINLINE const T* end() const
	{
		return m_Array + m_Count;
	}
};
