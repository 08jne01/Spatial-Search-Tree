#include "SpatialTree.h"
#include "intersects.h"
#include <assert.h>
SpatialTree::SpatialTree
(
	const float width,
	const float height
): 
	m_width(width),
	m_height(height)
{
	m_root = new BranchPoint(Rectangle(Vec2f(0, height), Vec2f(width, 0)));
}

SpatialTree::~SpatialTree()
{
	stack.clear();
	NodeIterator it(&stack, m_root);
	for (it.begin(); stack.size(); it.next())
	{
		if (it.current())
			delete it.current();
	}
}

void SpatialTree::draw(sf::RenderWindow& window)
{
	stack.clear();
	NodeIterator it(&stack, m_root);
	for (it.begin(); stack.size(); it.next())
	{
		if (!it.current())
			continue;
		drawShape(it.current()->m_rect, window, 255, 255, 255);
		for (Node* itemIt = it.current()->item(); itemIt; itemIt = itemIt->item())
		{
			drawShape(itemIt->m_rect, window, 255, 0, 0);
		}	
	}
}

bool SpatialTree::insert(Item* item, Node* start)
{
	if (!item)
		return false;

	stack.clear();
	NodeIterator it(&stack, start);
	bool left = true;
	bool right = true;
	bool inserted = false;
	bool inside = true;
	int i = 0;
	for (it.begin(); stack.size() && it.current(); it.next(left, right), i++)
	{
		left = false;
		right = false;

		inside = true;

		if (it.current()->left())
		{
			left = intersects(item->m_rect, it.current()->left()->m_rect);
			inside = left;
		}
		if (it.current()->right())
		{
			right = intersects(item->m_rect, it.current()->right()->m_rect);
			inside |= right;
		}

		if (!inside)
			break;

		if (it.current()->isLeaf()) //We are at the bottom
		{
			//Y or X split
			bool xSplit = true;
			float width = abs(it.current()->m_rect.m_ll.x - it.current()->m_rect.m_ur.x);
			float height = abs(it.current()->m_rect.m_ll.y - it.current()->m_rect.m_ur.y);
			if (width < height)
				xSplit = false;

			Rectangle r1;
			Rectangle r2;
			if (xSplit)
			{
				r1 = Rectangle(it.current()->m_rect.m_ll, Vec2f(it.current()->m_rect.m_ll.x + width / 2.0, it.current()->m_rect.m_ur.y));
				r2 = Rectangle(Vec2f(it.current()->m_rect.m_ur.x - width / 2.0, it.current()->m_rect.m_ll.y), it.current()->m_rect.m_ur);

			}
			else
			{
				r1 = Rectangle(it.current()->m_rect.m_ll, Vec2f(it.current()->m_rect.m_ur.x, it.current()->m_rect.m_ur.y + height/2.0));
				r2 = Rectangle(Vec2f(it.current()->m_rect.m_ll.x, it.current()->m_rect.m_ur.y + height/2.0), it.current()->m_rect.m_ur);
			}

			Node* newLeft = new BranchPoint(r1, it.current());
			Node* newRight = new BranchPoint(r2, it.current());
			it.current()->left(newLeft);
			it.current()->right(newRight);
			bool placedInLeft = false;
			if (intersects(it.current()->item()->m_rect, r1))
			{
				newLeft->item(it.current()->item());
				newLeft->item()->parent(newLeft);
				it.current()->item(nullptr);
				placedInLeft = true;
			}
			else
			{
				newRight->item(it.current()->item());
				newRight->item()->parent(newRight);
				it.current()->item(nullptr);
			}

			if (intersects(r1, item->m_rect))
			{
				if (placedInLeft)
				{
					if (i < maxDepth)
					{
						stack.push_back(newLeft);
					}
					else
					{
						inserted = addLinkedList(it.current()->left()->item(), item);
						break;
					}
				}
				else
				{
					newLeft->item(item);
					item->parent(newLeft);
					inserted = true;
					break;
				}
			}
			else
			{
				if (!placedInLeft)
				{
					if (i < maxDepth)
					{
						stack.push_back(newRight);
					}
					else
					{
						inserted = addLinkedList(it.current()->right()->item(), item);
						break;
					}	
				}
				else
				{
					newRight->item(item);
					item->parent(newRight);
					inserted = true;
					break;
				}
			}
		}
		else if (!left && !right)
		{
			it.current()->item(item);
			item->parent(it.current());
			inserted = true;
			break;
		}
		else
		{
			if (left)
			{
				stack.push_back(it.current()->left());
			}

			if (right)
			{
				stack.push_back(it.current()->right());
			}
		}
	}
	return inserted;
}

bool SpatialTree::remove(Item* item)
{
	bool removed = false;
	if (item->parent())
	{
		if (item->parent()->isBranchPoint() && !item->item())
		{
			return recursivelyRemove(item);
		}
		else
		{
			removeLinkedList(item);
		}
	}
	return removed;
}

bool SpatialTree::recursivelyRemove(Item* item)
{
	bool singleLeaf = true;
	Node* remainingLeaf = nullptr;
	Node* it = item->parent();
	if (it)
	{
		if (it->left() == item)
			remainingLeaf = it->right();
		else if (it->right() == item)
			remainingLeaf = it->left();
		else
			return false;
	}
	
	assert(it->isBranchPoint());

	Node* prevNode = nullptr;
	for (; it && singleLeaf; it = it->parent())
	{
		if (it->left() != remainingLeaf && it->left() || it->right != remainingLeaf && it->right())
		{
			return true;
		}
		
		if (it->parent()->left() == it)
		{
			it->parent()->left(remainingLeaf);
			remainingLeaf->parent(it->parent());
			delete it;
			it = remainingLeaf;
		}
		else if (it->parent()->right() == it)
		{
			it->parent()->right(remainingLeaf);
			remainingLeaf->parent(it->parent());
			delete it;
			it = remainingLeaf;
		}
	}

	return true;
}

bool SpatialTree::query(const Rectangle& area, std::vector<Item*>& items)
{
	NodeIterator it(&stack, m_root);
	for (it.begin(); it.current() && stack.size(); it.next())
	{

	}
}

bool SpatialTree::query(const Circle& area, std::vector<Item*>& items)
{

}

void SpatialTree::createNode(const Rectangle& rl, const Rectangle& rr, Node* current, Node* prevItem)
{
	Node* newLeft = new BranchPoint(rl);
	Node* newRight = new BranchPoint(rr);
	current->left(newLeft);
	current->right(newRight);
	if (intersects(rl, prevItem->m_rect))
	{
		newLeft->left(prevItem);
	}
	else
	{
		newRight->left(prevItem);
	}
}

BranchPoint::BranchPoint(const Rectangle& rect) :
	Node(rect)
{

}

BranchPoint::BranchPoint(const Rectangle& rect, Node* parent) :
	Node(rect, parent, nullptr)
{

}

BranchPoint::BranchPoint(const Rectangle& rect, Node* left, Node* right) :
	Node(rect), m_left(left), m_right(right)
{

}

BranchPoint::~BranchPoint()
{

}

Item::Item(const Rectangle& rect) :
	Node(rect, nullptr, nullptr)
{

}
Item::Item(const Rectangle& rect, Node* parent) :
	Node(rect, parent, nullptr)
{

}