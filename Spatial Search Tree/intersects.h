#pragma once
#include "Shapes.h"
extern bool intersects(const Rectangle& rect1, const Rectangle& rect2);
extern bool intersects(const Rectangle& rect, const Circle& circle);
extern bool intersects(const Rectangle& rect, const Point& point);
extern bool intersects(const Circle& circle, const Point& point);