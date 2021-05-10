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

	size_t GetChildCount() const
	{
		size_t count = 0;
		if (Left)
		{
			count += Left->GetChildCount() + 1;
		}
		if (Right)
		{
			count += Right->GetChildCount() + 1;
		}
		return count;
	}
};

/**
 * A generic binary tree implementation.
 * It's also a red black tree
 */
template <typename T, typename TNode = TBinaryTreeNode<T>, typename TCompare = FUtils::Less<T>, typename TAllocator =
          TRawAllocator<TNode>>
class TBinaryTree
{
	TCompare m_Compare{};
	TAllocator m_Allocator{};

	TNode* m_RootNode = nullptr;

	template <typename...Args>
	FORCEINLINE TNode* NewNode(Args& ...args)
	{
		TNode* res = m_Allocator.Alloc(1);
		new(res)TNode(args...);
		return res;
	}

	FORCEINLINE void RotateRight(TNode* root)
	{
		TNode* pivot = root->Left;
		root->Left = pivot->Right;

		if (pivot->Right)
		{
			pivot->Right->Parent = root;
		}
		pivot->Parent = root->Parent;

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

		pivot->Right = root;
		root->Parent = pivot;
	}

	FORCEINLINE void RotateLeft(TNode* root)
	{
		TNode* pivot = root->Right;
		root->Right = pivot->Left;

		if (pivot->Left)
		{
			pivot->Left->Parent = root;
		}
		pivot->Parent = root->Parent;

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

		pivot->Left = root;
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
						RotateRight(grandparent);
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

						RotateLeft(parent);
						RotateRight(grandparent);
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
						RotateLeft(grandparent);
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

						RotateRight(parent);
						RotateLeft(grandparent);
						node->IsRed = 0;
						grandparent->IsRed = 1;
					}
				}
			}
		}
	}

	template <bool TOrUpdate = false>
	FORCEINLINE bool InsertInternal(TNode** node, const T& data, TNode*& insertedNode)
	{
		TNode* parent = nullptr;

		while(*node)
		{
			if (m_Compare(data, (*node)->Data)) // presume <
			{
				parent = *node;
				node = &((*node)->Left);
			} else if(m_Compare((*node)->Data, data)) // presume >
			{
				parent = *node;
				node = &((*node)->Right);
			} else if constexpr (TOrUpdate) // presume ==
			{
				(*node)->Data = data;
				insertedNode = *node;
				return true;
			}
			else
			{
				return false;
			}
		}
		
		*node = NewNode(data);
		(*node)->Parent = parent;
		Balance(*node);
		insertedNode = *node;
		return true;
	}

	FORCEINLINE void FreeNode(TNode* n)
	{
		n->~TNode();
		m_Allocator.Free(n);
	}

	FORCEINLINE void TransplantNode(TNode* orig, TNode* rep)
	{
		if (!orig->Parent)
		{
			m_RootNode = rep;
		}
		else if (orig->Parent->IsLeftChild(orig))
		{
			orig->Parent->Left = rep;
		}
		else
		{
			orig->Parent->Right = rep;
		}
		rep->Parent = orig->Parent;
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

	FORCEINLINE void FixDoubleBlack(TNode* x)
	{
		// double black is fixed once the node is root or red-black
		while (x != m_RootNode && !x->IsRed)
		{
			if (x->Parent->IsLeftChild(x)) // if x is left child of its parent
			{
				TNode* w = x->Parent->Right; // w = x's sibling

				if (w->IsRed) // Case I
				{
					w->IsRed = 0;
					x->Parent->IsRed = 1;
					RotateLeft(x->Parent); // left rotation of the parent
					w = x->Parent->Right;
				}

				if (!w->Right->IsRed && !w->Left->IsRed) // if both children of w are black, Case II
				{
					w->IsRed = 1;
					x = x->Parent;
				}
				else if (!w->Right->IsRed) // if w's right child is black, Case III
				{
					w->Left->IsRed = 0;
					w->IsRed = 1;
					RotateRight(w);
					w = x->Parent->Right;
				}
				else
				{
					// Case IV
					w->IsRed = x->Parent->IsRed;
					x->Parent->IsRed = 0;
					w->Right->IsRed = 0;
					RotateLeft(x->Parent);
					x = m_RootNode;
				}
			}
			else // if x is right child of its parent
			{
				TNode* w = x->Parent->Left; // w = x's sibling

				if (w->IsRed) // Case I
				{
					w->IsRed = 0;
					x->Parent->IsRed = 1;
					RotateRight(x->Parent); // right rotation of the parent
					w = x->Parent->Left;
				}

				if (!w->Right->IsRed && !w->Left->IsRed) // if both children of w are black, Case II
				{
					w->IsRed = 1;
					x = x->Parent;
				}
				else if (!w->Left->IsRed) // if w's right child is black, Case III
				{
					w->Right->IsRed = 0;
					w->IsRed = 1;
					RotateLeft(w);
					w = x->Parent->Left;
				}
				else
				{
					// Case IV
					w->IsRed = x->Parent->IsRed;
					x->Parent->IsRed = 0;
					w->Left->IsRed = 0;
					RotateRight(x->Parent);
					x = m_RootNode;
				}
			}
		}
		x->IsRed = 0;
	}

public:
	struct InsertResult
	{
		bool Success;
		TNode* InsertedNode;
	};

	FORCEINLINE ~TBinaryTree()
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

	FORCEINLINE InsertResult Insert(const T& data)
	{
		TNode* insertedNode;
		return {InsertInternal(&m_RootNode, data, insertedNode), insertedNode};
	}

	FORCEINLINE InsertResult InsertOrUpdate(const T& data)
	{
		TNode* insertedNode;
		return {InsertInternal<true>(&m_RootNode, data, insertedNode), insertedNode};
	}

	template <typename U>
	FORCEINLINE TNode* FindNode(const U& data, TNode* node)
	{
		while (node)
		{
			if (m_Compare(data, node->Data)) // presume <
			{
				node = node->Left;
			}
			else if (m_Compare(node->Data, data)) // presume >
			{
				node = node->Right;
			} else
			{
				return node;
			}
		}
		return node;
	}

	template <typename U>
	FORCEINLINE TNode* FindNode(const U& data)
	{
		return FindNode(data, m_RootNode);
	}

	FORCEINLINE void DeleteNode(TNode* n)
	{
		if (!n)return;

		char originalIsRed = n->IsRed;
		TNode* x;

		if (!n->Left)
		{
			x = n->Right;
			TransplantNode(n, x);
		}
		else if (!n->Right)
		{
			x = n->Left;
			TransplantNode(n, x);
		}
		else
		{
			TNode* y = n->Right->GetMinValueNode();
			originalIsRed = y->IsRed;
			x = y->Right;

			if (y->Parent == n)
			{
				x->Parent = y;
			}
			else
			{
				TransplantNode(y, y->Right);
				y->Right = n->Right;
				y->Right->Parent = y;
			}

			TransplantNode(n, y);
			y->Left = n->Left;
			y->Left->Parent = y;
			y->IsRed = n->IsRed;
		}

		FreeNode(n);
		if (!originalIsRed) // the node is now double-black which violates the rules
		{
			FixDoubleBlack(x);
		}
	}

	FORCEINLINE NODISCARD size_t GetNodeCount() const
	{
		return m_RootNode ? m_RootNode->GetChildCount() + 1 : 0;
	}

	FORCEINLINE NODISCARD size_t GetBlackHeight() const
	{
		TNode* n = m_RootNode;
		size_t height = 0;

		while (n)
		{
			if (!n->IsRed)
				++height;

			if (n->Left)
			{
				n = n->Left;
			}
			else if (n->Right)
			{
				n = n->Right;
			}
			else
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
