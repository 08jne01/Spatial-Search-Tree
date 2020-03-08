#pragma once
#include <assert.h>
#include <Maths/Maths.h>
struct Rectangle
{
	Rectangle(Vec2f ll, Vec2f ur):
		m_ll(ll), m_ur(ur)
	{
		//assert(ll.x <= ur.x && ll.y >= ur.y);
		if (ll.x <= ur.x && ll.y >= ur.y)
		{

		}
		else
		{
			volatile int x = 10;
		}
	}
	Rectangle()
	{

	}
	Vec2f m_ll;
	Vec2f m_ur;
};

struct Circle
{
	Circle(Vec2f pos, int r):
		m_pos(pos), m_r(r)
	{

	}
	Circle()
	{

	}
	Vec2f m_pos;
	double m_r;
};

struct Point
{
	Point(Vec2f pos) :
		m_pos(pos)
	{

	}
	Vec2f m_pos;
};

namespace sf
{
	class RenderWindow;
}

extern void drawShape(const Rectangle& rect, sf::RenderWindow& window, unsigned char r, unsigned char g, unsigned char b);
extern void drawShape(const Circle& circle, sf::RenderWindow& window, unsigned char r, unsigned char g, unsigned char b);