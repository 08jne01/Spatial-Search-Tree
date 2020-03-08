#include "Program.h"
#include "intersects.h"
#include "Shapes.h"
#include "SpatialTree.h"
Program::Program
(
	const short width,
	const short height
) :
	m_width(width),
	m_height(height)
{

}

int Program::mainLoop()
{
	window.create(sf::VideoMode(m_width, m_height), "Spatial Search Tree Example");
	if (!window.isOpen())
	{
		return EXIT_FAILURE;
	}

	sf::Event events;
	sf::Clock clock;
	Rectangle boundary(Vec2f(500, 250), Vec2f(600, 200));
	//circle.setRadius(20);
	SpatialTree tree(m_width, m_height);
	while (window.isOpen())
	{
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		Rectangle mouseBoundary(Vec2f(pos.x-50, pos.y + 50), Vec2f(pos.x + 50, pos.y -50));

		while (window.pollEvent(events))
		{
			if (events.type == sf::Event::EventType::Closed)
			{
				window.close();
			}
			
			if (events.type == sf::Event::EventType::MouseButtonPressed)
			{
				Item* item = new Item(Rectangle(Vec2f(pos.x - 10, pos.y + 10), Vec2f(pos.x + 10, pos.y - 10)));
				if (!tree.insert(item))
				{
					std::cout << "Failed to insert" << std::endl;
					delete item;
				}	
				else
					std::cout << "Inserted Point" << std::endl;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			Item* item = new Item(Rectangle(Vec2f(pos.x - 10, pos.y + 10), Vec2f(pos.x + 10, pos.y - 10)));
			if (!tree.insert(item))
			{
				std::cout << "Failed to insert" << std::endl;
				delete item;
			}
				
		}

		window.clear();
		tree.draw(window);
		if (intersects(boundary, mouseBoundary))
		{
			drawShape(mouseBoundary, window, 255, 0, 0);
		}
		else
		{
			drawShape(mouseBoundary, window, 255, 255, 255);
		}
		drawShape(boundary, window, 255, 255, 255);
		window.display();
	}

	return EXIT_SUCCESS;
}

void drawShape(const Rectangle& rect, sf::RenderWindow& window, unsigned char r, unsigned char g, unsigned char b)
{
	sf::RectangleShape drawRect;
	drawRect.setPosition(rect.m_ll.x, rect.m_ur.y);
	drawRect.setSize(sf::Vector2f(rect.m_ur.x - rect.m_ll.x, rect.m_ll.y - rect.m_ur.y));
	drawRect.setFillColor(sf::Color(0, 0, 0, 0));
	drawRect.setOutlineColor(sf::Color(r, g, b));
	drawRect.setOutlineThickness(1);
	window.draw(drawRect);
}

void drawShape(const Circle& circle, sf::RenderWindow& window, unsigned char r, unsigned char g, unsigned char b)
{
	sf::CircleShape drawCirc;
	drawCirc.setRadius(circle.m_r);
	drawCirc.setPosition(circle.m_pos.x - drawCirc.getGlobalBounds().width/2.0, circle.m_pos.y - drawCirc.getGlobalBounds().height/2.0);
	drawCirc.setFillColor(sf::Color(0, 0, 0, 0));
	drawCirc.setOutlineColor(sf::Color(r,g,b));
	drawCirc.setOutlineThickness(1);
	window.draw(drawCirc);
}