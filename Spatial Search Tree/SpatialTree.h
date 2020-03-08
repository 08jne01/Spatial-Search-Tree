#pragma once
#include <Maths/Maths.h>
#include <vector>
#include "Shapes.h"
class Node
{
public:
	Node(const Rectangle& rect) : m_rect(rect)
	{

	}
	Node(
		const Rectangle& rect,
		Node* parentPtr,
		Node* itemPtr
	) : 
		m_rect(rect),
		m_parent(parentPtr),
		m_item(itemPtr)
	{

	}
	virtual inline Node* left() const = 0;
	virtual inline Node* right() const = 0;
	//virtual inline Node* parent() const = 0;
	//virtual inline Node* item() const = 0;
	//virtual inline Node* nextItem() const = 0;
	//virtual inline Node* prevItem() const = 0;
	virtual inline void left(Node* ptrL) = 0;
	virtual inline void right(Node* ptrR) = 0;
	//virtual inline void parent(Node* ptr) = 0;
	//virtual inline void item(Node* item) = 0;
	virtual inline bool isLeaf() const = 0;
	virtual inline bool isBranchPoint() const = 0;
	virtual inline bool bottom() const = 0;
	//virtual inline void nextItem(Node* item) = 0;
	//virtual inline void prevItem(Node* item) = 0;
	Rectangle m_rect;

	inline void parent(Node* ptr)
	{
		m_parent = ptr;
	}
	inline void item(Node* ptr)
	{
		m_item = ptr;
	}
	inline Node* parent() const
	{
		return m_parent;
	}
	inline Node* item() const
	{
		return m_item;
	}
private:
	Node* m_parent;
	Node* m_item;
};

class NodeIterator
{
public:
	NodeIterator(std::vector<Node*>* stack):
		m_stack(stack)
	{

	}
	NodeIterator(std::vector<Node*>* stack, Node* start) :
		m_stack(stack), m_current(start)
	{
		if (m_current)
			m_stack->push_back(m_current);
	}
	NodeIterator()
	{

	}
	inline Node* next()
	{
		m_current = m_stack->back();
		m_stack->pop_back();
		if (m_current)
		{
			m_stack->push_back(m_current->left());
			m_stack->push_back(m_current->right());
		}
		return m_current;
	}
	inline Node* next(bool left, bool right)
	{
		m_previous = m_current;
		m_current = m_stack->back();
		m_stack->pop_back();

		if (!left && !right)
			return m_current;

		if (m_current)
		{
			if (left)
				m_stack->push_back(m_current->left());
			if (right)
				m_stack->push_back(m_current->right());
		}
		return m_current;
	}
	inline Node* begin()
	{
		return next();
	}
	inline Node* begin(bool left, bool right)
	{
		return next(left, right);
	}

	inline Node* current()
	{
		return m_current;
	}
	inline Node* previous()
	{
		return m_previous;
	}
private:
	Node* m_current = nullptr;
	Node* m_previous = nullptr;
	std::vector<Node*>* m_stack;
};

class BranchPoint : public Node
{
public:
	BranchPoint(const Rectangle& rect);
	BranchPoint(const Rectangle& rect, Node* parent);
	BranchPoint(const Rectangle& rect, Node* left, Node* right);
	~BranchPoint();

	inline bool isLeaf() const
	{
		return item() != nullptr;
	}
	inline Node* left() const
	{
		return m_left;
	}
	inline Node* right() const
	{
		return m_right;
	}
	inline bool bottom() const
	{
		return !m_left && !m_right;
	}
	inline void left(Node* ptrL)
	{
		m_left = ptrL;
	}
	inline void right(Node* ptrR)
	{
		m_right = ptrR;
	}
	inline bool isBranchPoint() const
	{
		return true;
	}
private:
	Node* m_left = nullptr;
	Node* m_right = nullptr;
};

class Item : public Node
{
public:
	Item(const Rectangle& rect);
	Item(const Rectangle& rect, Node* parent);
	inline bool isLeaf() const
	{
		return true;
	}
	inline Node* left() const
	{
		return nullptr;
	}
	inline Node* right() const
	{
		return nullptr;
	}
	inline Node* item() const
	{
		return (Node*)this;
	}
	inline void left(Node* ptrL)
	{
		return;
	}
	inline void right(Node* ptrR)
	{
		return;
	}
	inline bool bottom() const
	{
		return true;
	}
	inline bool isBranchPoint() const
	{
		return false;
	}
private:
	//Node* m_next = nullptr; //for close objects.
	//Node* m_prev = nullptr;
	//Node* m_parent = nullptr;
};

class SpatialTree
{
public:
	SpatialTree(const float width, const float height);
	~SpatialTree();
	void draw(sf::RenderWindow& window);
	inline bool insert(Item* item);
	bool insert(Item* item, Node* start);
	bool remove(Item* item);
	bool query(const Rectangle& area, std::vector<Item*>& items);
	bool query(const Circle& area, std::vector<Item*>& items);
	bool recursivelyRemove(Item* item);
	void createNode(const Rectangle& rl, const Rectangle& rr, Node* current, Node* prevItem);
	inline bool addLinkedList(Node* start, Node* item);
	inline bool removeLinkedList(Node* item);
private:
	Node* m_root;
	const float m_width;
	const float m_height;
	std::vector<Node*> stack; //scratch stack
	const unsigned char maxDepth = 20;
};

inline bool SpatialTree::addLinkedList(Node* start, Node* item)
{
	assert(start->isLeaf());
  	for (Node* itemIt = start; itemIt; itemIt = itemIt->item())
	{
		if (!itemIt->item())
		{
			itemIt->item(item);
			item->parent(itemIt);
			return true;
		}
	}
	return false;
}

inline bool SpatialTree::removeLinkedList(Node* item)
{
	if (item)
	{
		assert(item->parent());
		item->parent()->item(item->item());
		if (item->item())
		{
			item->item()->parent(item->parent());
		}
	}
	return true;
}

inline bool SpatialTree::insert(Item* item)
{
	return insert(item, m_root);
}