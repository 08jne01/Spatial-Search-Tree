#pragma once
#include <SFML/Graphics.hpp>
#include "Shapes.h"

struct Rectangle;
struct Circle;

class Program
{
public:
	Program(const short width, const short height);
	int mainLoop();
	
private:
	short m_width; //Width of the window
	short m_height; //Height of the window
	sf::RenderWindow window;
};