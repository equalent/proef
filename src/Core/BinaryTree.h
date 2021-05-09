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

template <typename T>
struct TBinaryTreeNode
{
	TBinaryTreeNode* Left;
	TBinaryTreeNode* Parent;
	TBinaryTreeNode* Right;
	char IsRed;

	T Data;

	explicit TBinaryTreeNode(const T& data) : Left(nullptr), Parent(nullptr), Right(nullptr), IsRed(1), Data(data)
	{
	}

	TBinaryTreeNode* GetOtherChild(TBinaryTreeNode* x)
	{
		if (x == Left)
		{
			return Right;
		}

		return Left;
	}

	bool IsEmpty() const
	{
		return !Right && !Left;
	}

	/**
	 * Returns the next node in ascending order
	 */
	TBinaryTreeNode* GetInorderSuccessor()
	{
		if (Right) // if node has right subtree, find the minimum value there
		{
			return Right->GetMinValueNode();
		}
		else
		{
			// find the first node which is a left child of its parent
			
			TBinaryTreeNode* n = this;
			TBinaryTreeNode* p = Parent;

			while (p && (n == p->Right))
			{
				n = p;
				p = p->Parent;
			}
			return p;
		}
	}

	/**
	 * Returns the next node in descending order
	 */
	TBinaryTreeNode* GetInorderPredeccessor()
	{
		if (Left) // if node has left subtree, find the maximum value there
		{
			return Left->GetMaxValueNode();
		}
		else
		{
			// find the first node which is a right child of its parent
			
			TBinaryTreeNode* n = this;
			TBinaryTreeNode* p = Parent;

			while (p && (n == p->Left))
			{
				n = p;
				p = p->Parent;
			}
			return p;
		}
	}

	TBinaryTreeNode* GetMinValueNode()
	{
		TBinaryTreeNode* cur = this;

		// when a node has no subtree on the left, it's the smallest node in the tree
		while (cur->Left)
		{
			cur = cur->Left;
		}

		return cur;
	}

	TBinaryTreeNode* GetMaxValueNode()
	{
		TBinaryTreeNode* cur = this;

		// when a node has no subtree on the right, it's the largest node in the tree
		while (cur->Right)
		{
			cur = cur->Right;
		}

		return cur;
	}

	bool IsLeftChild(const TBinaryTreeNode* x) const
	{
		return x == Left;
	}

#if 0
	// untested function
	void ReplaceWith(TBinaryTreeNode* other)
	{
		if(Parent)
		{
			if(Parent->IsLeftChild(this))
			{
				Parent->Left = other;
			} else
			{
				Parent->Right = other;
			}
		}

		if(other->Parent)
		{
			if (other->Parent->IsLeftChild(other))
			{
				other->Parent->Left = nullptr;
			}
			else
			{
				other->Parent->Right = nullptr;
			}
		}

		other->Parent = Parent;
		Parent = nullptr;
	}
#endif
};

/**
 * A generic binary tree implementation.
 * It's also a red black tree
 */
template <typename T, typename TNode = TBinaryTreeNode<T>, typename TCompare = FUtils::Less<T>, typename TAllocator =
          TRawAllocator<TNode>>
class TBinaryTree
{
	TCompare m_Compare;
	TAllocator m_Allocator;

	TNode* m_RootNode = nullptr;

	template <typename...Args>
	FORCEINLINE TNode* NewNode(Args ...args)
	{
		TNode* res = m_Allocator.Alloc(1);
		new(res)TNode(args...);
		return res;
	}

	FORCEINLINE void RotateTree(bool right, TNode* root)
	{
		TNode* pivot = right ? root->Left : root->Right;
		(right ? root->Left : root->Right) = right ? pivot->Right : pivot->Left;
		(right ? pivot->Right : pivot->Left) = root;

		if (root->Parent)
		{
			if (root->Parent->IsLeftChild(root))
			{
				root->Parent->Left = pivot;
			}
			else
			{
				root->Parent->Right = pivot;
			}
		}
		else
		{
			m_RootNode = pivot;
		}

		pivot->Parent = root->Parent;
		root->Parent = pivot;
	}

	/**
	 * Balance the tree using red black tree algorithm
	 */
	FORCEINLINE void Balance(TNode* node)
	{
		if (node == m_RootNode)
		{
			node->IsRed = 0; // root node is always black
		}
		else if (node->Parent->IsRed) // if X's parent is red
		{
			if (node->Parent->Parent) // if X has a grandparent
			{
				TNode* parent = node->Parent;
				TNode* grandparent = parent->Parent;
				TNode* aunt = node->Parent->Parent->GetOtherChild(node->Parent);
				//if (!aunt)return;

				if (aunt && aunt->IsRed) // aunt is red
				{
					parent->IsRed = 0;
					aunt->IsRed = 0;
					grandparent->IsRed = 1;
					Balance(grandparent); // repeat coloring with X = grandparent
				}
				else // aunt is black
				{
					const bool parentLeft = grandparent->IsLeftChild(parent);
					const bool xLeft = parent->IsLeftChild(node);

					if (parentLeft && xLeft) // Left Left Case
					{
						/*
						 * LL rotation:
						 * 1. Right rotation of grandparent
						 * 2. Swap grandparent and parent colors
						 */
						RotateTree(true, grandparent);
						parent->IsRed = 0;
						grandparent->IsRed = 1;
					}
					else if (parentLeft && !xLeft) // Left Right Case
					{
						/*
						 * LR rotation:
						 * 1. Left rotation of parent
						 * 2. Right rotation of grandparent
						 * 3. Swap grandparent and parent colors
						 */

						RotateTree(false, parent);
						RotateTree(true, grandparent);
						node->IsRed = 0;
						grandparent->IsRed = 1;
					}
					else if (!parentLeft && !xLeft) // Right Right Case
					{
						/*
						 * RR rotation:
						 * 1. Left rotation of grandparent
						 * 2. Swap grandparent and parent colors
						 */
						RotateTree(false, grandparent);
						parent->IsRed = 0;
						grandparent->IsRed = 1;
					}
					else // Right Left Case
					{
						/*
						 * RL rotation:
						 * 1. Right rotation of parent
						 * 2. Left rotation of grandparent
						 * 3. Swap grandparent and parent colors
						 */

						RotateTree(true, parent);
						RotateTree(false, grandparent);
						node->IsRed = 0;
						grandparent->IsRed = 1;
					}
				}
			}
		}
	}

	template <bool TOrUpdate = false>
	FORCEINLINE bool InsertInternal(TNode*& node, TNode* parent, const T& data)
	{
		if (!node)
		{
			node = NewNode(data);
			node->Parent = parent;
			Balance(node);
			return true;
		}
		else if (m_Compare(data, node->Data)) // presume <
		{
			return InsertInternal(node->Left, node, data);
		}
		else if (m_Compare(node->Data, data)) // presume >
		{
			return InsertInternal(node->Right, node, data);
		}
		else if constexpr (TOrUpdate) // presume ==
		{
			node->Data = data;
			return true;
		}
		else
		{
			return false;
		}
	}

	FORCEINLINE void FreeNode(TNode* n)
	{
		n->~TNode();
		m_Allocator.Free(n);
	}

	FORCEINLINE void DeleteTree(TNode* n)
	{
		if (!n)return;

		TNode* nLeft = n->Left;
		TNode* nRight = n->Right;

		FreeNode(n);
		DeleteTree(nLeft);
		DeleteTree(nRight);
	}

public:
	~TBinaryTree()
	{
		Clear();
	}

	FORCEINLINE void Clear()
	{
		// recursion, unfortunately
		// better that using custom dynamic stack on the heap
		DeleteTree(m_RootNode);
		m_RootNode = nullptr;
	}

	FORCEINLINE bool Insert(const T& data)
	{
		return InsertInternal(m_RootNode, nullptr, data);
	}

	FORCEINLINE bool InsertOrUpdate(const T& data)
	{
		return InsertInternal<true>(m_RootNode, nullptr, data);
	}

	template<typename U>
	FORCEINLINE TNode* FindNode(const U& data, TNode* node)
	{
		if (!node)
		{
			return nullptr;
		}
		else if (m_Compare(data, node->Data)) // presume <
		{
			return FindNode(data, node->Left);
		}
		else if (m_Compare(node->Data, data)) // presume >
		{
			return FindNode(data, node->Right);
		}
		else // presume ==
		{
			return node;
		}
	}

	template<typename U>
	FORCEINLINE TNode* FindNode(const U& data)
	{
		return FindNode(data, m_RootNode);
	}

	FORCEINLINE void DeleteNode(TNode* n)
	{
		if (!n) return;

		if (!n->Left && !n->Right) // node with leaves only: simply delete
		{
			if (n->Parent) // if node has a parent, mark the node as a leaf
			{
				if (n->Parent->IsLeftChild(n))
				{
					n->Parent->Left = nullptr;
				}
				else
				{
					n->Parent->Right = nullptr;
				}
			}
			FreeNode(n);
		}
		else if (n->Left && n->Right) // node has two children
		{
			if (n->Right->IsEmpty()) // right child empty: replace self data with its data
			{
				n->Data = (T&&)n->Right->Data; // move data from the right one
				DeleteNode(n->Right);
			}
			else
			{
				TNode* inorderSuccessor = n->GetInorderSuccessor();
				n->Data = (T&&)inorderSuccessor->Data;
				DeleteNode(inorderSuccessor);
			}
		}
		else if (n->Left) // only left child
		{
			n->Data = (T&&)n->Left->Data;
			DeleteNode(n->Left);
		}
		else // only right child
		{
			n->Data = (T&&)n->Right->Data;
			DeleteNode(n->Right);
		}
	}

	FORCEINLINE NODISCARD size_t GetNodeCount() const
	{
		TNode* n = m_RootNode;
		size_t count = 0;

		while (n)
		{
			++count;
			n = n->Left;
		}

		n = m_RootNode;

		while (n)
		{
			++count;
			n = n->Right;
		}

		return count;
	}

	FORCEINLINE NODISCARD size_t GetBlackHeight() const
	{
		TNode* n = m_RootNode;
		size_t height = 0;

		while (n)
		{
			if(!n->IsRed)
				++height;

			if(n->Left)
			{
				n = n->Left;
			} else if(n->Right)
			{
				n = n->Right;
			} else
			{
				break;
			}
		}

		return height;
	}

	FORCEINLINE NODISCARD TNode* GetRootNode() const
	{
		return m_RootNode;
	}
};
