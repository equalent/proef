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
 * A simple map implementation based on red black binary search tree
 */
template <typename TKey, typename TValue, typename TCompare = FUtils::Less<const TKey>, typename TPair = TPair<TKey, TValue>, typename TAllocator =
          TRawAllocator<TBinaryTreeNode<TPair>>>
class TMap
{
	using CompareType = TPairFirstCompare<TKey, TValue, TCompare>;

public: // make tree type public for external tree inspection
	using TreeNodeType = TBinaryTreeNode<TPair>;
	using TreeType = TBinaryTree<TPair, TreeNodeType, CompareType, TAllocator>;

	class Iterator
	{
		friend TMap;
		
		TreeNodeType* m_Node;
		
		explicit Iterator(TreeNodeType* node) : m_Node(node){}

	public:
		void operator++()
		{
			check(m_Node);
			m_Node = m_Node->GetInorderSuccessor();
		}

		TPair& operator*()
		{
			check(m_Node);
			return m_Node->Data;
		}

		const TPair& operator*() const
		{
			check(m_Node);
			return m_Node->Data;
		}

		bool operator==(const Iterator& other) const
		{
			return m_Node == other.m_Node;
		}

		bool operator!=(const Iterator& other) const
		{
			return m_Node != other.m_Node;
		}
	};

	class ReverseIterator
	{
		friend TMap;

		TreeNodeType* m_Node;

		explicit ReverseIterator(TreeNodeType* node) : m_Node(node) {}

	public:
		void operator++()
		{
			check(m_Node);
			m_Node = m_Node->GetInorderPredeccessor();
		}

		TPair& operator*()
		{
			check(m_Node);
			return m_Node->Data;
		}

		const TPair& operator*() const
		{
			check(m_Node);
			return m_Node->Data;
		}

		bool operator==(const ReverseIterator& other) const
		{
			return m_Node == other.m_Node;
		}

		bool operator!=(const ReverseIterator& other) const
		{
			return m_Node != other.m_Node;
		}
	};

private:
	TreeType m_Tree;

public:
	TMap() = default;

	void Insert(const TKey& key, const TValue& value)
	{
		m_Tree.Insert(TPair(key, value));
	}

	void InsertOrUpdate(const TKey& key, const TValue& value)
	{
		m_Tree.InsertOrUpdate(TPair(key, value));
	}

	size_t GetCount() const
	{
		return m_Tree.GetNodeCount();
	}

	TreeType& GetTree()
	{
		return m_Tree;
	}

	void Clear()
	{
		m_Tree.Clear();
	}

	TValue& operator[](const TKey& key)
	{
		TreeNodeType* node = m_Tree.FindNode(key);
		return node->Data.Second;
	}

	Iterator begin()
	{
		return Iterator(m_Tree.GetRootNode()->GetMinValueNode());
	}

	Iterator end()
	{
		return Iterator(nullptr);
	}

	const Iterator& begin() const
	{
		return Iterator(m_Tree.GetRootNode()->GetMinValueNode());
	}

	const Iterator& end() const
	{
		return Iterator(nullptr);
	}
	

	ReverseIterator rbegin()
	{
		return ReverseIterator(m_Tree.GetRootNode()->GetMaxValueNode());
	}

	ReverseIterator rend()
	{
		return ReverseIterator(nullptr);
	}

	const ReverseIterator& rbegin() const
	{
		return ReverseIterator(m_Tree.GetRootNode()->GetMaxValueNode());
	}

	const ReverseIterator& rend() const
	{
		return ReverseIterator(nullptr);
	}
};
