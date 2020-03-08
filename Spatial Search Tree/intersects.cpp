#include "intersects.h"
#include "Shapes.h"

static inline float clamp(float value, float min, float max)
{
	return std::max(min, std::min(max, value));
}

bool intersects(const Rectangle& rect1, const Rectangle& rect2)
{
	return !(rect1.m_ll.x > rect2.m_ur.x ||
		rect1.m_ur.x < rect2.m_ll.x ||
		rect1.m_ll.y < rect2.m_ur.y ||
		rect1.m_ur.y > rect2.m_ll.y
		);
}

bool intersects(const Rectangle& rect, const Circle& circle)
{
	float closeX = circle.m_pos.x - clamp(circle.m_pos.x, rect.m_ll.x, rect.m_ur.x);
	float closeY = circle.m_pos.y - clamp(circle.m_pos.y, rect.m_ur.y, rect.m_ll.y);
	return (closeX * closeX + closeY * closeY) < (circle.m_r * circle.m_r);
}

bool intersects(const Rectangle& rect, const Point& point)
{
	return point.m_pos.x >= rect.m_ll.x &&
		point.m_pos.x <= rect.m_ur.x &&
		point.m_pos.y >= rect.m_ur.y && //Inverted y coordinate as standard.
		point.m_pos.y <= rect.m_ll.y;
}

bool intersects(const Circle& circle, const Point& point)
{
	return distanceSquared(circle.m_pos, point.m_pos) <= circle.m_r*circle.m_r;
}